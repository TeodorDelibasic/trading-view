package tradingView;

import java.awt.Color;
import java.awt.Graphics;

public class Candlestick {

	public Candlestick(double open, double close, double high, double low) {
		this.open = Math.round(open * 100.0) / 100.0;
		this.close = Math.round(close * 100.0) / 100.0;
		this.high = Math.round(high * 100.0) / 100.0;
		this.low = Math.round(low * 100.0) / 100.0;
	}
	
	public void draw(Stock s, int number, int count, double highest) {
		Graphics g = s.getGraphics();
		g.setColor((open < close) ? new Color(68, 168, 50) : Color.RED);
		
		int w = (int) (1.0 / s.getWidthRatio()); // Sirina svece
		int h = (int) (Math.abs(open - close) / s.getHeightRatio()); // Visina svece
		
		int x = number * w; // Pozicija svece: x
		int y = (int) ((highest - ((open < close) ? close : open)) / s.getHeightRatio()); // Pozicija svece: y
		
		g.fillRect(x + w / 8, y, 3 * w / 4, h);
		
		h = (int) ((high - low) / s.getHeightRatio()); // Visina linije
		
		x = number * w + w / 2; // Pozicija linije: x
		y = (int) ((highest - high) / s.getHeightRatio()); // Pozicija linije: y
		
		g.drawLine(x, y, x, y + h);
	}

	public double getHigh() {
		return this.high;
	}
	
	public double getLow() {
		return this.low;
	}
	
	public double getClose() {
		return this.close;
	}
	
	@Override
	public String toString() {
		return String.format("%7.3f %7.3f %7.3f %7.3f", open, close, high, low);
	}
	
	private double open, close;
	private double high, low;
}
