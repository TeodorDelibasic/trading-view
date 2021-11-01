package tradingView;

public class Crawler {
	
	private Crawler() {}
	
	public static String makeURL(StockInterval si) {
		return "https://query1.finance.yahoo.com/v8/finance/chart/" + si.symbol +
				"?period1=" + si.start +
				"&period2=" + si.end +
				"&interval=1h";
	}
	
	public static native String getInfo(String url);
	
	public static void main(String[] args) {
		System.loadLibrary("POOP_Projekat2_cpp");
		
		System.out.println(getInfo("https://query1.finance.yahoo.com/v8/finance/chart/aapl?period1=1616072670&period2=1617531870&interval=1h"));
	}
}
