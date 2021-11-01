package tradingView;

import java.util.Map;

public abstract class Indicator {
	public abstract void draw(int n, Map<Integer, Candlestick> candles, Stock s, int count, double highest);
}
