package tradingView;

public class CurrentUser {
	private static CurrentUser instance = null;
	
	private CurrentUser() { }
  
    public static CurrentUser getInstance() {
        if (instance == null)
            instance = new CurrentUser();
        
        return instance;
    }
    
    public boolean getLoggedIn() {
		return this.loggedIn;
	}
    
	public User getUser() {
		return this.current;
	}

	public void buying(double price) {
		this.current.buying(price);
	}
	
	public void selling(double price) {
		this.current.selling(price);
	}

	void logIn(User user) {
		this.loggedIn = true;
		this.current = user;
	}
	void logOut() {
		this.loggedIn = false;
		this.current = null;
	}
    
    private User current = null;
    private boolean loggedIn = false;
}
