package tradingView.Errors;

public class RegisterError extends Exception {
	public RegisterError() {
		super("Korisnicko ime vec postoji!");
	}
}
