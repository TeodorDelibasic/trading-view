package tradingView;

public class User {
	
	public User(String username, String password, double balance) {
		this.username = username;
		this.password = password;
		this.balance = balance;
	}
	
	public User(String username, String password) {
		this(username, password, 0);
	}
	
	public String getUsername() { return this.username; }
	public String getPassword() { return this.password; }
	public double getBalance() { return this.balance; }
	
	public void setBalance(double balance) { this.balance = balance; }
	public void buying(double price) { this.balance -= price; }
	public void selling(double price) { this.buying(-price); }
	
	private String username;
	private String password;
	private double balance;
	
}
