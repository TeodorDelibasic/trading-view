package tradingView;

public class Interval {
	
	public Interval() { }
	
	public Interval(int start, int end) {
		this.start = start;
		this.end = end;
	}
	
	@Override
	public String toString() {
		return start + " " + end;
	}
	
	int start, end;
	
	public static void main(String[] args) {
		Interval i1 = new Interval(2, 11);
		Interval i2 = new Interval(1, 10);
	}
}
