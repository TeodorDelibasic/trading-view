package tradingView.Errors;

public class InsufficientSharesError extends Exception {
	public InsufficientSharesError() {
		super("Nemate dovoljno trazenih akcija!");
	}
}
