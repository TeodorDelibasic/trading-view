package tradingView;

public class StockInterval extends Interval {
	
	public StockInterval(String symbol, int start, int end) {
		super(start, end);
		this.symbol = symbol;
	}
	
	public StockInterval(String symbol, Interval i) {
		this(symbol, i.start, i.end);
	}

	String symbol;
}
