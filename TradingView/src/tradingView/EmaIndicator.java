package tradingView;

import java.awt.Color;
import java.awt.Graphics;
import java.util.Map;
import java.util.Map.Entry;

public class EmaIndicator extends Indicator {
	public void draw(int n, Map<Integer, Candlestick> candles, Stock s, int count, double highest) {
		if(s.showEma) {
			Graphics g = s.getGraphics();
			g.setColor(Color.MAGENTA);
			double sum = 0, emaJuce = 0;;
			int i = 0;
			for(Entry<Integer, Candlestick> e : candles.entrySet()) {
				if(s.showing.start <= e.getKey() && e.getKey() <= s.showing.end) {
					
					if(i < n) {
						sum = sum + e.getValue().getClose();
						emaJuce = sum / n;
					}
					else {
						double emaDanas = e.getValue().getClose() * 2.0 / (n + 1) + emaJuce * (1 - 2.0 / (n + 1));
						
						int w = (int) (1.0 / s.getWidthRatio()); // Sirina svece
						
						int x1 = i * w;
						int y1 = (int) ((highest - emaJuce) / s.getHeightRatio());
						
						int x2 = (i + 1) * w;
						int y2 = (int) ((highest - emaDanas) / s.getHeightRatio());
						
						g.drawLine(x1 + w / 2, y1, x2 + w / 2, y2);
						
						emaJuce = emaDanas;
					}
					i = i + 1;
				}
			}
		}
	}
	public static void main(String[] args) {
		System.out.println(5 * 2.0 / 5 + 5 * (1 - 2.0 / 5));
	}
}
