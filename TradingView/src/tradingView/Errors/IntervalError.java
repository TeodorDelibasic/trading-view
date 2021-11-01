package tradingView.Errors;

public class IntervalError extends Exception {
	public IntervalError() {
		super("Krajnji datum ne sme biti veci od pocetnog!");
	}
}
