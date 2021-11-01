package tradingView;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;
import java.util.Map.Entry;

public class MaIndicator extends Indicator {
	public void draw(int n, Map<Integer, Candlestick> candles, Stock s, int count, double highest) {
		if(s.showMa) {
			Graphics g = s.getGraphics();
			g.setColor(Color.BLUE);
			double sum = 0;
			int i = 0;
			Iterator<Map.Entry<Integer, Candlestick>> itr = candles.entrySet().iterator();
			for(Entry<Integer, Candlestick> e : candles.entrySet()) {
				if(s.showing.start <= e.getKey() && e.getKey() <= s.showing.end) {
					if(i < n)
						sum = sum + e.getValue().getClose();
					else {
						double ma1 = sum / n;
						
						sum = sum + e.getValue().getClose();
						sum = sum - itr.next().getValue().getClose();
						
						double ma2 = sum / n;
						
						int w = (int) (1.0 / s.getWidthRatio()); // Sirina svece
						
						int x1 = i * w;
						int y1 = (int) ((highest - ma1) / s.getHeightRatio());
						
						int x2 = (i + 1) * w;
						int y2 = (int) ((highest - ma2) / s.getHeightRatio());
						
						g.drawLine(x1 + w / 2, y1, x2 + w / 2, y2);
					}
					i = i + 1;
				}
				else {
					itr.next();
				}
			}
		}
	}
}
