package tradingView;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import tradingView.Errors.CurlError;

public class Parser {
	
	public static void checkError(String json) throws CurlError {
		if(!Pattern.compile("\"error\":null").matcher(json).find()) {
			Matcher m = Pattern.compile("\"description\":\"(.*)\"").matcher(json);
			if(m.find())
				throw new CurlError(m.group());
		}
	}

	public static Map<Integer, Candlestick> getCandles(String json){
		Map<Integer, Candlestick> candles = new TreeMap<>();

		List<Pattern> pl = new ArrayList<>();
		pl.add(Pattern.compile("\"timestamp\":\\[([^\\]]*)\\]"));
		pl.add(Pattern.compile("\"high\":\\[([^\\]]*)\\]"));
		pl.add(Pattern.compile("\"low\":\\[([^\\]]*)\\]"));
		pl.add(Pattern.compile("\"open\":\\[([^\\]]*)\\]"));
		pl.add(Pattern.compile("\"close\":\\[([^\\]]*)\\]"));
		
		List<Matcher> ml1 = new ArrayList<>();
		pl.forEach(p -> ml1.add(p.matcher(json)));
		
		Pattern p = Pattern.compile("[\\d\\.]+");
		
		boolean check = true;
		
		List<Matcher> ml2 = new ArrayList<>();
		for(Matcher m : ml1) {
			if(!m.find())
				check = false;
			else
				ml2.add(p.matcher(m.group(1)));
		}
		
		label: while(check) {
			for(Matcher m : ml2) {
				if(!m.find())
					break label;
			}
			
			int ts = Integer.parseInt(ml2.get(0).group());
			
			double high = Double.parseDouble(ml2.get(1).group());
			double low = Double.parseDouble(ml2.get(2).group());
			double open = Double.parseDouble(ml2.get(3).group());
			double close = Double.parseDouble(ml2.get(4).group());
			
			Candlestick cs = new Candlestick(open, close, high, low);
			
			candles.put(ts, cs);
		}
		
		return candles;
	}
	
	public static void main(String[] args) {
		System.loadLibrary("POOP_Projekat2_cpp");
		
		String json = Crawler.getInfo("https://query1.finance.yahoo.com/v8/finance/chart/aapl?period1=1616072670&period2=1617531870&interval=1h");
		getCandles(json);
	}
}
