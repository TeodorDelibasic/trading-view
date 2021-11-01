package tradingView.Errors;

public class NegativeBalanceError extends Exception {
	public NegativeBalanceError() {
		super("Nije dozvoljen negativan iznos!");
	}
}
