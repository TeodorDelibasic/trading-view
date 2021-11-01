package tradingView.Errors;

public class CredentialsError extends Exception {
	public CredentialsError() {
		super("Morate uneti korisnicko ime i lozinku!");
	}
}
