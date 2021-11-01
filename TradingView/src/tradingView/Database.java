package tradingView;

public class Database {
	private static Database instance = null;
	
	private Database() { }
	private static native void createConnection();
  
    public static Database getInstance() {
        if (instance == null) {
            instance = new Database();
            createConnection();
        }
        return instance;
    }
    
    public native boolean checkUser(User user);
    public native boolean checkUsername(String username);
    public native void insertUser(User user);
    
    public native int getUserId(String username);
    public native int getStockId(String symbol);
    
    public native boolean checkIfStockExists(String symbol);
    public native void addStock(String symbol);
    public native boolean checkIfOwnsExists(int idUser, int idStock);
    public native void addOwnsStock(int idUser, int idTrans);
    public native void insertBuying(int idTrans, int amount);
    
    public native String getSymbol(int idTrans);
    public native int checkHas(int idBought, int amount);
    public native double getProfit(int idBought, double price);
    public native void updateBuying(int idBought, int amount);
    public native boolean checkIfNotSold(int idUser, int idStock);
    public native void removeOwnsStock(int idUser, int idStock);
    public native void insertSelling(int idTrans, double profit, int idBought);
    
    public native int lastTransId();
    public native void insertTransaction(double price, int amount, int idUser, int idStock);
    public native void updateBalance(int idUser, double balance);
    
    public native double getBalance(int idUser);
    public native String getOwnedStocks(int idUser);
    public native String getBought(int idUser);
    public native String getSold(int idUser);
    
    
    public static void main(String[] args) {
    	System.loadLibrary("POOP_Projekat2_cpp");
    	
    	User u1 = new User("Teodor", "Delibasic", 1000);
    	
    	Database db = Database.getInstance();
    	System.out.println(db.checkIfStockExists("aapl"));
	}
}
