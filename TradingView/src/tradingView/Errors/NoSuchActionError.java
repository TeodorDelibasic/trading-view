package tradingView.Errors;

public class NoSuchActionError extends Exception {
	public NoSuchActionError() {
		super("Niste kupovali akcije sa tim rednim brojem transakcije!");
	}
}
