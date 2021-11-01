package tradingView;

public class Pair<E1, E2> {
	
	public Pair(E1 e1, E2 e2) {
		this.e1 = e1;
		this.e2 = e2;
	}
	
	@Override
	public String toString() {
		return "" + e1;
	}
	
	E1 e1;
	E2 e2;
}
