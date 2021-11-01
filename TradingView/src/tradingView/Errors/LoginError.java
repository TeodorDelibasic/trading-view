package tradingView.Errors;

public class LoginError extends Exception {
	public LoginError() {
		super("Pogresno korisnicko ime ili lozinka!");
	}
}
