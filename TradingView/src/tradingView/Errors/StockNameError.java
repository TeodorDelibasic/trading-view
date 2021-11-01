package tradingView.Errors;

public class StockNameError extends Exception {
	public StockNameError() {
		super("Nepostojece ime akcije!");
	}
}
