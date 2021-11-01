package tradingView;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;

import tradingView.Errors.CurlError;

import java.util.NavigableMap;
import java.util.TreeMap;

public class Stock extends Canvas {
	
	
	// Unutrasnja klasa za drag i zoom
	private class MouseEventHandler extends MouseAdapter implements MouseWheelListener {
		
		private int startX, lastX;
		
		public void mouseDragged(MouseEvent e) {
			// Koliko treba timestampova za jednu svecu
			int timestampsPerCandle = (showing.end - showing.start) / getNumberOf();
			// Koliko sveca treba nadoknaditi
			int brojSveca = (int) (Math.abs(lastX - e.getX()) * widthRatio);
			// Koliko treba timestampova da se nadoknade svece
			int timestampOffset = timestampsPerCandle * brojSveca;
			
			if(e.getX() < lastX)
				timestampOffset = -timestampOffset;
			
			showing.start = showing.start - timestampOffset;
			showing.end = showing.end - timestampOffset;
			
			lastX = e.getX();
			
			repaint();
			revalidate();
		}

		public void mousePressed(MouseEvent e) {
			startX = lastX = e.getX();
		}

		public void mouseReleased(MouseEvent e) {
			// Koliko treba timestampova za jednu svecu
			int timestampsPerCandle = (showing.end - showing.start) / getNumberOf();
			// Koliko sveca treba nadoknaditi
			int brojSveca = (int) (Math.abs(startX - e.getX()) * widthRatio);
			// Koliko treba timestampova da se nadoknade svece
			int timestampOffset = timestampsPerCandle * brojSveca;
			
			if(e.getX() < startX)
				timestampOffset = -timestampOffset;
			
			showing.start = showing.start - timestampOffset;
			showing.end = showing.end - timestampOffset;
			
			try {
				addCandles(showing);
			} catch (CurlError e1) { }
			
			repaint();
			revalidate();
		}
		
		@Override
		public void mouseWheelMoved(MouseWheelEvent e) {
			if (e.getWheelRotation() < 0) {
				showing.start += 2 * (showing.end - showing.start) / getNumberOf();
				showing.end -= 2 * (showing.end - showing.start) / getNumberOf();
			} else {
				showing.start -= 2 * (showing.end - showing.start) / getNumberOf();
				showing.end += 2 * (showing.end - showing.start) / getNumberOf();
			}
			try {
				addCandles(showing);
			} catch (CurlError e1) {
			}
			repaint();
			revalidate();
		}
	}
	
	// Atributi
	
	// Prikaz
	Interval showing;
	private double widthRatio, heightRatio;
	private double zoom = 1;
	
	// Svece
	private String symbol;
	private Map<Integer, Candlestick> candles = new TreeMap<>();
	private List<Pair<Integer, Boolean>> timestamps = new ArrayList<>();
	boolean showMa, showEma;

	// Konstruktor
	public Stock(String symbol) throws CurlError {
		this.symbol = symbol;
		
		setBackground(Color.LIGHT_GRAY);
		
		int current = (int) (System.currentTimeMillis() / 1000L); 
		showing = new Interval(current - 30 * 86400, current);
		
		addCandles(showing);
		
		MouseEventHandler meh = new MouseEventHandler();
		addMouseMotionListener(meh);
		addMouseListener(meh);
		addMouseWheelListener(meh);
	}
	
	public void setMa(boolean check) {
		this.showMa = check;
	}
	
	public void setEma(boolean check) {
		this.showEma = check;
	}
	
	@Override
	public void paint(Graphics g) {
		
		int i = 0;
		int cnt = getNumberOf();
		
		double highest = getHighest();

		widthRatio = zoom * getNumberOf() / getWidth();
		heightRatio = zoom * Math.ceil(getHighest() - getLowest()) / getHeight();
		
		new MaIndicator().draw(10, candles, this, cnt, highest);
		new EmaIndicator().draw(10, candles, this, cnt, highest);
		
		for(Entry<Integer, Candlestick> e : candles.entrySet()) {
			if(showing.start <= e.getKey() && e.getKey() <= showing.end)
				e.getValue().draw(this, i++, cnt, highest);
		}
	}
	
	// Proporcije za crtanje
	public double getHeightRatio() {
		return this.heightRatio;
	}
	public double getWidthRatio() {
		return this.widthRatio;
	}
	
	// Broj iscrtanih sveca
	public int getNumberOf() {
		int cnt = 0;
		
		for(Entry<Integer, Candlestick> e : candles.entrySet()) {
			if(showing.start <= e.getKey() && e.getKey() <= showing.end)
				cnt = cnt + 1;
		}
		
		return (cnt > 0) ? cnt : 1;
	}
	
	// Najniza i najvisa sveca
	public double getLowest() {
		double lowest = Double.POSITIVE_INFINITY;
		
		for(Entry<Integer, Candlestick> c : candles.entrySet()) {
			if(showing.start <= c.getKey() && c.getKey() <= showing.end)
				lowest = (lowest < c.getValue().getLow()) ? lowest : c.getValue().getLow();
		}
		
		return lowest;
	}
	public double getHighest() {
		double highest = 0;
		
		for(Entry<Integer, Candlestick> c : candles.entrySet()) {
			if(showing.start <= c.getKey() && c.getKey() <= showing.end)
				highest = (highest > c.getValue().getHigh()) ? highest : c.getValue().getHigh();
		}
		
		return highest;
	}
	
	// Rad sa svecama
	public void addCandles(Interval i) throws CurlError {
		List<Interval> newIntervals = difference(i);
		
		for(Interval ni : newIntervals) {
			String url = Crawler.makeURL(new StockInterval(symbol, ni));
			String json = Crawler.getInfo(url);
			
			Parser.checkError(json);
			Parser.getCandles(json).entrySet().forEach(c -> candles.put(c.getKey(), c.getValue()));
		}
		
		union(i);
	}
	public void showInterval(Interval i) throws CurlError {
		addCandles(i);
		showing = i;
		repaint();
	}
	public double getCurrentPrice() {
		int current = (int) (System.currentTimeMillis() / 1000L);
		
		try {
			addCandles(new Interval(current - 30 * 86400, current));
		} catch (CurlError e) {}
		
		NavigableMap<Integer, Candlestick> map = (NavigableMap<Integer, Candlestick>) candles;
		return map.lastEntry().getValue().getClose();
	}
	
	// Rad sa intervalima
	private void union(Interval interval) {
		int i = 0;
		
		for(; i < timestamps.size() && timestamps.get(i).e1 < interval.start; i++);
		timestamps.add(i, new Pair<Integer, Boolean>(interval.start, true));
		
		for(; i < timestamps.size() && timestamps.get(i).e1 <= interval.end; i++);
		timestamps.add(i, new Pair<Integer, Boolean>(interval.end, false));
		
		List<Pair<Integer, Boolean>> newTimestamps = new ArrayList<>();
		
		int depth = 0;
		for(Pair<Integer, Boolean> p : timestamps) {
			if(p.e2 == true) {
				if(depth == 0)
					newTimestamps.add(p);
				depth = depth + 1;
			}
			else {
				if(depth == 1)
					newTimestamps.add(p);
				depth = depth - 1;
			}
		}
		
		timestamps = newTimestamps;
	}
	private List<Interval> difference(Interval interval) {
		List<Pair<Pair<Integer, Boolean>, Boolean>> newTimestamps = new ArrayList<>();
		timestamps.forEach(ts -> {
			newTimestamps.add(new Pair<Pair<Integer, Boolean>, Boolean>(ts, true));
		});
		
		int i = 0;
		
		for(i = 0; i < newTimestamps.size() && 
				newTimestamps.get(i).e1.e1 < interval.start; i++);
		newTimestamps.add(i, 
				new Pair<>(new Pair<>(interval.start, true), false));
		
		for(i = 0; i < newTimestamps.size() && 
				newTimestamps.get(i).e1.e1 <= interval.end; i++);
		newTimestamps.add(i, 
				new Pair<>(new Pair<>(interval.end, false), false));
		
		List<Interval> intervals = new ArrayList<>();
		
		int depth = 0, begin = 0;
		for(Pair<Pair<Integer, Boolean>, Boolean> p : newTimestamps) {
			if(depth == 1 && begin != p.e1.e1) {
				if(p.e2 == true && p.e1.e2 == true)
					intervals.add(new Interval(begin, p.e1.e1 - 1));
				else if(p.e2 == false && p.e1.e2 == false)
					intervals.add(new Interval(begin, p.e1.e1));
			}
			
			if(p.e1.e2 == true)
				depth = depth + 1;
			else
				depth = depth - 1;
			
			if(depth == 1) {
				if(p.e2 == true)
					begin = p.e1.e1 + 1;
				else
					begin = p.e1.e1;
			}
		}
		
		return intervals;
	}
	
	// Ispis
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		
		sb.append(symbol + "\n");
		
		for(Map.Entry<Integer, Candlestick> entry : candles.entrySet()) {
			sb.append(entry.getKey() + " " + entry.getValue() + "\n");
		}
		
		return sb.toString();
	}
	public void printTimestamps() {
		timestamps.forEach(i -> System.out.println(i));
	}
	
	// Main
	public static void main(String[] args) {
		System.loadLibrary("POOP_Projekat2_cpp");
		
		Stock s = null;
		
		try {
			s = new Stock("aapl");
		} catch (CurlError e) {
			System.out.println(e);
		}
		
		System.out.println(s);
		System.out.println(s.getCurrentPrice());
	}
}
