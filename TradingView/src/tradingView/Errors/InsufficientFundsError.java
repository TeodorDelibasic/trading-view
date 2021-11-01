package tradingView.Errors;

public class InsufficientFundsError extends Exception {
	public InsufficientFundsError() {
		super("Nemate dovoljno para!");
	}
}
