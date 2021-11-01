package tradingView;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.CardLayout;
import java.awt.Checkbox;
import java.awt.Choice;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Label;
import java.awt.List;
import java.awt.Menu;
import java.awt.MenuBar;
import java.awt.MenuItem;
import java.awt.MenuShortcut;
import java.awt.Panel;
import java.awt.TextField;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.time.LocalDate;
import java.time.ZoneId;
import java.time.ZoneOffset;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import tradingView.Errors.CredentialsError;
import tradingView.Errors.CurlError;
import tradingView.Errors.DateError;
import tradingView.Errors.InsufficientFundsError;
import tradingView.Errors.InsufficientSharesError;
import tradingView.Errors.LoginError;
import tradingView.Errors.NegativeBalanceError;
import tradingView.Errors.NoSuchActionError;
import tradingView.Errors.RegisterError;

public class StockExchange extends Frame {
	
	// Izbor izmedju sign in i register
	private Panel choicePanel() {
		Panel g1 = new Panel(new GridLayout(0, 1));
		g1.setBackground(Color.LIGHT_GRAY);
		
		Panel upperPanel = new Panel(new BorderLayout());
		Panel lowerPanel = new Panel(new BorderLayout());
		
		Panel f1 = new Panel(new FlowLayout());
		Panel f2 = new Panel(new FlowLayout());
		
		Button b1 = new Button("Register");
		b1.addActionListener(ae -> {
			((CardLayout) top.getLayout()).show(top, "register");
		});
		
		Button b2 = new Button("Sign in");
		b2.addActionListener(ae -> {
			((CardLayout) top.getLayout()).show(top, "login");
		});
		
		f1.add(b2);
		f2.add(b1);
		
		upperPanel.add(f1, BorderLayout.SOUTH);
		lowerPanel.add(f2, BorderLayout.NORTH);
		
		g1.add(upperPanel);
		g1.add(lowerPanel);
		
		g1.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentShown(ComponentEvent e) {
				b2.requestFocusInWindow();
			}
		});
		
		return g1;
	}
	
	// Log in
	private Panel loginPanel() {
		Panel g1 = new Panel(new GridLayout(0, 1));
		g1.setBackground(Color.LIGHT_GRAY);
		
		Panel upperPanel = new Panel(new BorderLayout());
		Panel lowerPanel = new Panel(new BorderLayout());
		
		Panel t = new Panel(new GridLayout(0, 1));
		
		Panel f1 = new Panel(new FlowLayout());
		Panel f2 = new Panel(new FlowLayout());
		Panel f3 = new Panel(new FlowLayout());
		Panel f4 = new Panel(new FlowLayout());
		
		Label userLabel = new Label("Username:");
		TextField username = new TextField(7);
		
		Label passLabel = new Label("Password:");
		TextField password = new TextField(7);
		password.setEchoChar('*');
		
		Button b1 = new Button("Sign in");
		b1.addActionListener(ae -> {
			
			try {
				if(username.getText().length() > 0 && password.getText().length() > 0) {
					
					User user = new User(username.getText(), password.getText());
					
					if(Database.getInstance().checkUser(user)) {
						
						int idUser = Database.getInstance().getUserId(username.getText());
						double balance = Database.getInstance().getBalance(idUser);
						user.setBalance(balance);
						
						CurrentUser.getInstance().logIn(user);
						
						this.username.setText(user.getUsername());
						this.balance.setText("" + user.getBalance());
						
						Pattern p = Pattern.compile("(.*)\\n");
						
						Matcher m1 = p.matcher(Database.getInstance().getOwnedStocks(idUser));
						actions.removeAll();
						while(m1.find()) {
							actions.add(m1.group());
						}
						
						Matcher m2 = p.matcher(Database.getInstance().getBought(idUser));
						purchases.removeAll();
						while(m2.find()) {
							purchases.add(m2.group());
						}
						
						Matcher m3 = p.matcher(Database.getInstance().getSold(idUser));
						sales.removeAll();
						while(m3.find()) {
							sales.add(m3.group());
						}
						
						setErrorLabel("");
						
						mi.setEnabled(true);
						
						((CardLayout) top.getLayout()).show(top, "main");
						
					} else {
						throw new LoginError();
					}
				} else {
					throw new CredentialsError();
				}
			} catch(Exception e) { 
				setErrorLabel(e.getMessage());
			}
		});
		
		Button b2 = new Button("Back");
		b2.addActionListener(ae -> {
			sales.removeAll();
			purchases.removeAll();
			actions.removeAll();
			
			((CardLayout) top.getLayout()).show(top, "choice");
			
			setErrorLabel("");
		});
		
		f1.add(userLabel);
		f1.add(username);
		
		f2.add(passLabel);
		f2.add(password);
		
		f3.add(b1);
		f4.add(b2);
		
		t.add(f1);
		t.add(f2);
		t.add(f3);
		t.add(f4);
		
		upperPanel.add(t, BorderLayout.SOUTH);
		lowerPanel.add(errorLabel, BorderLayout.SOUTH);
		
		g1.add(upperPanel);
		g1.add(lowerPanel);
		
		g1.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentShown(ComponentEvent e) {
				username.requestFocusInWindow();
			}
		});
		
		return g1;
	}

	// Register
	private Panel registerPanel() {
		Panel g1 = new Panel(new GridLayout(0, 1));
		g1.setBackground(Color.LIGHT_GRAY);
		
		Panel upperPanel = new Panel(new BorderLayout());
		Panel lowerPanel = new Panel(new BorderLayout());
		
		Panel t = new Panel(new GridLayout(0, 1));
		
		Panel f1 = new Panel(new FlowLayout());
		Panel f2 = new Panel(new FlowLayout());
		Panel f3 = new Panel(new FlowLayout());
		Panel f4 = new Panel(new FlowLayout());
		Panel f5 = new Panel(new FlowLayout());
		
		Label userLabel = new Label("Username:");
		TextField username = new TextField(7);
		f1.add(userLabel);
		f1.add(username);
		
		Label passLabel = new Label("Password:");
		TextField password = new TextField(7);
		password.setEchoChar('*');
		f2.add(passLabel);
		f2.add(password);
		
		Label balanceLabel = new Label("   Balance:");
		TextField balance = new TextField(7);
		f3.add(balanceLabel);
		f3.add(balance);
		
		Button b1 = new Button("Register");
		b1.addActionListener(ae -> {
			
			try {
				if(username.getText().length() > 0 && password.getText().length() > 0) {
					if(!Database.getInstance().checkUsername(username.getText())) {
						double balanceVal = 1000;
						
						if(balance.getText().length() > 0) {
							balanceVal = Double.parseDouble(balance.getText());
						}
						
						if(balanceVal < 0) 
							throw new NegativeBalanceError();
						
						User user = new User(username.getText(), password.getText(), balanceVal);
						
						Database.getInstance().insertUser(user);
						
						CurrentUser.getInstance().logIn(user);
						
						this.username.setText(user.getUsername());
						this.balance.setText("" + user.getBalance());
						
						((CardLayout) top.getLayout()).show(top, "main");
						
						mi.setEnabled(true);
						
						setErrorLabel("");
						
					} else {
						throw new RegisterError();
					}
				} else {
					throw new CredentialsError();
				}
			} catch(NumberFormatException e) {
				
				System.out.println("Here");
				setErrorLabel("Nepravilan iznos!");
				
			} catch(Exception e) { 
				setErrorLabel(e.getMessage());
			}
		});
		f4.add(b1);
		
		Button b2 = new Button("Back");
		b2.addActionListener(ae -> {
			((CardLayout) top.getLayout()).show(top, "choice");
			
			setErrorLabel("");
		});
		f5.add(b2);
		
		t.add(f1);
		t.add(f2);
		t.add(f3);
		t.add(f4);
		t.add(f5);
		
		upperPanel.add(t, BorderLayout.SOUTH);
		lowerPanel.add(errorLabel, BorderLayout.SOUTH);
		
		g1.add(upperPanel);
		g1.add(lowerPanel);
		
		g1.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentShown(ComponentEvent e) {
				username.requestFocusInWindow();
			}
		});
		
		return g1;
	}
	
	// Popunjavanje istoka
	private Panel stockPanel() {
		Panel b = new Panel(new BorderLayout());
		
		TextField newStock = new TextField(5);
		
		Button addStock = new Button("Add stock");
		addStock.addActionListener(ae -> {
			try {
				String symbol = newStock.getText();
				
				stocks.put(symbol, new Stock(symbol));
				stockChoice.insert(symbol, 0);
				currentStock.setText(symbol);
				
				graph.add(stocks.get(symbol), symbol);
				((CardLayout) graph.getLayout()).show(graph, symbol);
				
				newStock.setText("");
				
				setErrorLabel("");
			} catch(Exception e) {
				setErrorLabel(e.getMessage());
			}
		});
		
		Panel f1 = new Panel(new FlowLayout());
		f1.add(stockChoice);
		
		Panel f2 = new Panel(new FlowLayout());
		f2.add(newStock);
		f2.add(addStock);
		
		b.add(currentStock, BorderLayout.NORTH);
		b.add(f1);
		b.add(f2, BorderLayout.SOUTH);
		
		return b;
	}
	private Panel intervalPanel() {
		Panel g = new Panel(new GridLayout(0, 1));
		
		Label startLabel = new Label("Start date:");
		TextField startText = new TextField(8);
		
		Label endLabel = new Label("End date:");
		TextField endText = new TextField(8);
		
		Button show = new Button("Show");
		show.addActionListener(ae -> {
			try {
				Pattern p = Pattern.compile("^(\\d{1,2})\\.(\\d{1,2})\\.(\\d\\d\\d\\d)\\.$");
				
				Matcher m1 = p.matcher(startText.getText());
				Matcher m2 = p.matcher(endText.getText());
				
				if(m1.matches() && m2.matches()) {
					LocalDate d1 = LocalDate.of(Integer.parseInt(m1.group(3)), 
							Integer.parseInt(m1.group(2)), 
							Integer.parseInt(m1.group(1)));
					
					ZoneId zone1 = ZoneId.of("Europe/Belgrade");
					ZoneOffset zoneOffSet1 = zone1.getRules().getOffset(d1.atStartOfDay());
					
					int start = (int) d1.atStartOfDay().toEpochSecond(zoneOffSet1);
					
					LocalDate d2 = LocalDate.of(Integer.parseInt(m2.group(3)), 
							Integer.parseInt(m2.group(2)), 
							Integer.parseInt(m2.group(1)));
					
					ZoneId zone2 = ZoneId.of("Europe/Belgrade");
					ZoneOffset zoneOffSet2 = zone2.getRules().getOffset(d2.atStartOfDay());
					
					int end = (int) d2.atStartOfDay().toEpochSecond(zoneOffSet2);
					
					if(start > end)
						throw new InternalError();
					
					Stock s = stocks.get(currentStock.getText());
					s.showInterval(new Interval(start, end));
				} else {
					throw new DateError();
				}
				
				setErrorLabel("");
				
			} catch(Exception e) {
				setErrorLabel(e.getMessage());
			}
		});
		
		Panel f1 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		f1.add(startLabel);
		f1.add(startText);
		
		Panel f2 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		f2.add(endLabel);
		f2.add(endText);
		
		Panel f3 = new Panel(new FlowLayout());
		f3.add(show);
		
		g.add(f1);
		g.add(f2);
		g.add(f3);
		
		return g;
	}
	private Panel indicatorPanel() {
		Panel f = new Panel(new FlowLayout());
		
		Checkbox ma = new Checkbox("MA", false);
		ma.addItemListener(ie -> {
			stocks.get(currentStock.getText()).setMa(ma.getState());
			stocks.get(currentStock.getText()).repaint();
			stocks.get(currentStock.getText()).revalidate();
		});
		Checkbox ema = new Checkbox("EMA", false);
		ema.addItemListener(ie -> {
			stocks.get(currentStock.getText()).setEma(ema.getState());
			stocks.get(currentStock.getText()).repaint();
			stocks.get(currentStock.getText()).revalidate();
		});
		
		TextField n = new TextField(5);
		
		f.add(ma);
		f.add(ema);
		//f.add(n);
		
		return f;
	}
	private Panel transactionPanel() {
		Panel g = new Panel(new GridLayout(0, 1));
		
		Label symbolLabel = new Label("Symbol(Buying only):");
		TextField symbolText = new TextField(5);
		
		Label amountLabel = new Label("Amount:");
		TextField amountText = new TextField(5);
		
		Label idLabel = new Label("ID(Selling only):");
		TextField idText = new TextField(5);
		
		Button buy = new Button("Buy");
		buy.addActionListener(ae -> {
			try {
				
				int amount = Integer.parseInt(amountText.getText());
				
				String symbol = symbolText.getText();
				double price = new Stock(symbol).getCurrentPrice();
				
				if(CurrentUser.getInstance().getUser().getBalance()	< amount * price)
					throw new InsufficientFundsError();
				
				if(!Database.getInstance().checkIfStockExists(symbol))
					Database.getInstance().addStock(symbol);
				
				int idUser = Database.getInstance().getUserId(CurrentUser.getInstance().getUser().getUsername());
				int idStock = Database.getInstance().getStockId(symbol);
				
				CurrentUser.getInstance().buying(amount * price);
				Database.getInstance().updateBalance(idUser, CurrentUser.getInstance().getUser().getBalance());
				
				if(!Database.getInstance().checkIfOwnsExists(idUser, idStock))
					Database.getInstance().addOwnsStock(idUser, idStock);
				
				Database.getInstance().insertTransaction(price, amount, idUser, idStock);
				int idTrans = Database.getInstance().lastTransId();
				Database.getInstance().insertBuying(idTrans, amount);
				
				balance.setText(String.format("%.2f", Database.getInstance().getBalance(idUser)));
				
				Pattern p = Pattern.compile("(.*)\\n");
				
				Matcher m1 = p.matcher(Database.getInstance().getOwnedStocks(idUser));
				actions.removeAll();
				while(m1.find()) {
					actions.add(m1.group());
				}
				
				Matcher m2 = p.matcher(Database.getInstance().getBought(idUser));
				purchases.removeAll();
				while(m2.find()) {
					purchases.add(m2.group());
				}
				
				setErrorLabel("");
			} catch(NumberFormatException e ) {
				setErrorLabel("Neispravna kolicina!");
			} catch (Exception e) {
				setErrorLabel(e.getMessage());
			}
			
		});
		
		Button sell = new Button("Sell");
		sell.addActionListener(ae -> {
			try {
				int amount = Integer.parseInt(amountText.getText());
				int idBought = Integer.parseInt(idText.getText());
				
				String symbol = Database.getInstance().getSymbol(idBought);
				if(symbol.length() == 0)
					throw new NoSuchActionError();
				double price = new Stock(symbol).getCurrentPrice();
				
				int left = Database.getInstance().checkHas(idBought, amount);
				if(left < 0)
					throw new InsufficientSharesError();
				Database.getInstance().updateBuying(idBought, left);
				
				double profit = Database.getInstance().getProfit(idBought, price);
				
				int idUser = Database.getInstance().getUserId(CurrentUser.getInstance().getUser().getUsername());
				int idStock = Database.getInstance().getStockId(symbol);
				
				CurrentUser.getInstance().selling(amount * price);
				Database.getInstance().updateBalance(idUser, CurrentUser.getInstance().getUser().getBalance());
				
				if(!Database.getInstance().checkIfNotSold(idUser, idStock))
					Database.getInstance().removeOwnsStock(idUser, idStock);
				
				Database.getInstance().insertTransaction(price, amount, idUser, idStock);
				int idTrans = Database.getInstance().lastTransId();
				Database.getInstance().insertSelling(idTrans, profit, idBought);
				
				
				balance.setText(String.format("%.2f", Database.getInstance().getBalance(idUser)));
				
				Pattern p = Pattern.compile("(.*)\\n");
				
				Matcher m1 = p.matcher(Database.getInstance().getOwnedStocks(idUser));
				actions.removeAll();
				while(m1.find()) {
					actions.add(m1.group());
				}
				
				Matcher m2 = p.matcher(Database.getInstance().getSold(idUser));
				sales.removeAll();
				while(m2.find()) {
					sales.add(m2.group());
				}
				
				Matcher m3 = p.matcher(Database.getInstance().getBought(idUser));
				purchases.removeAll();
				while(m3.find()) {
					purchases.add(m3.group());
				}
				
				setErrorLabel("");
			} catch(NumberFormatException e ) {
				setErrorLabel("Neispravna kolicina ili ID transakcije!");
			} catch (Exception e) {
				setErrorLabel(e.getMessage());
			}
		});
		
		Panel f1 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		f1.add(symbolLabel);
		f1.add(symbolText);
		
		Panel f2 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		f2.add(amountLabel);
		f2.add(amountText);
		
		Panel f3 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		f3.add(idLabel);
		f3.add(idText);
		
		Panel f4 = new Panel(new FlowLayout());
		f4.add(buy);
		f4.add(sell);
		
		g.add(f2);
		g.add(f1);
		g.add(f3);
		g.add(f4);
		
		return g;
	}
	private Panel infoPanel() {
		Panel g1 = new Panel(new GridLayout(0, 1, -5, -5));
		
		Panel f1 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		Panel f2 = new Panel(new FlowLayout(FlowLayout.RIGHT));
		
		Label userLabel = new Label("Username:");
		Label balanceLabel = new Label("Balance:");
		
		f1.add(userLabel);
		f1.add(username);
		
		f2.add(balanceLabel);
		f2.add(balance);
		
		g1.add(f1);
		g1.add(f2);
		
		return g1;
	}
	
	private Panel mainPanelEast() {
		Panel g = new Panel(new GridLayout(0, 1));
		g.setBackground(Color.GRAY);
		
		Panel temp;
		
		temp = new Panel(new BorderLayout());
		temp.add(stockPanel(), BorderLayout.NORTH);
		g.add(temp);
		
		temp = new Panel(new BorderLayout());
		temp.add(intervalPanel(), BorderLayout.NORTH);
		g.add(temp);
		
		temp = new Panel(new FlowLayout());
		temp.add(indicatorPanel());
		g.add(temp);
		
		temp = new Panel(new BorderLayout());
		temp.add(transactionPanel(), BorderLayout.NORTH);
		g.add(temp);
		
		temp = new Panel(new BorderLayout());
		temp.add(infoPanel(), BorderLayout.SOUTH);
		g.add(temp);
		
		return g;
	}
	
	// Popunjavanje juga
	private Panel portfolioPanel() {
		Panel g = new Panel(new GridLayout(1, 0));
		g.setBackground(Color.DARK_GRAY);
		
		Label o = new Label("Owned stocks");
		o.setForeground(Color.GREEN);
		
		Panel b1 = new Panel(new BorderLayout());
		b1.add(o, BorderLayout.NORTH);
		b1.add(actions, BorderLayout.SOUTH);
		g.add(b1);
		
		Label p = new Label("Purchases");
		p.setForeground(Color.GREEN);
		
		Panel b2 = new Panel(new BorderLayout());
		b2.add(p, BorderLayout.NORTH);
		b2.add(purchases, BorderLayout.SOUTH);
		g.add(b2);
		
		Label s = new Label("Sales");
		s.setForeground(Color.GREEN);
		
		Panel b3 = new Panel(new BorderLayout());
		b3.add(s, BorderLayout.NORTH);
		b3.add(sales, BorderLayout.SOUTH);
		g.add(b3);
		
		return g;
	}
	
	private Panel mainPanelSouth() {
		Panel b1 = new Panel(new BorderLayout());
		b1.setBackground(Color.DARK_GRAY);
		
		b1.add(portfolioPanel(), BorderLayout.NORTH);
		b1.add(errorLabel, BorderLayout.SOUTH);
		
		return b1;
	}
	
	// Popunjavanje main
	private Panel mainPanel() {
		Panel b1 = new Panel(new BorderLayout());
		
		b1.add(graph, BorderLayout.CENTER);
		b1.add(mainPanelEast(), BorderLayout.EAST);
		b1.add(mainPanelSouth(), BorderLayout.SOUTH);
		
		return b1;
	}
	
	// Dodavanje menija
	private void addMenu() {
		MenuBar bar = new MenuBar();
		setMenuBar(bar);
		
		Menu menu = new Menu("Menu");
		bar.add(menu);
		
		menu.add(mi);
	}
	
	// Podesavanje poruke sa greskom
	private void setErrorLabel(String msg) {
		errorLabel.setText(msg); 
		errorLabel.setBackground((msg.length() > 0) ? Color.RED : Color.DARK_GRAY);
		errorLabel.revalidate();
	}
	
	// Atributi
	private Map<String, Stock> stocks = new HashMap<>();
	
	private MenuItem mi = new MenuItem("Sign out");
	
	private Panel top = new Panel(new CardLayout());
	private Label errorLabel = new Label();
	
	// Center
	private Panel graph = new Panel(new CardLayout());
	
	// East
	private Label currentStock = new Label("aapl");
	private Choice stockChoice = new Choice();

	// South
	private Label username = new Label();
	private Label balance = new Label();
	
	private List actions = new List(5);
	private List purchases = new List(5);
	private List sales = new List(5);
	
	// Podesavanje atributa
	{
		try {
			stocks.put("aapl", new Stock("aapl"));
			graph.add(stocks.get("aapl"), "aapl");
		} catch (CurlError e) { }
		
		currentStock.setFont(new Font(null, Font.BOLD, 25));
		currentStock.setAlignment(Label.CENTER);
		
		stockChoice.add("aapl");
		stockChoice.addItemListener(ie -> {
			currentStock.setText(stockChoice.getSelectedItem());
			
			((CardLayout) graph.getLayout()).show(graph, stockChoice.getSelectedItem());
		});
		
		mi.addActionListener(ae -> {
			CurrentUser.getInstance().logOut();
			
			purchases.removeAll();
			sales.removeAll();
			actions.removeAll();
			
			mi.setEnabled(false);
			
			setErrorLabel("");
			
			((CardLayout) top.getLayout()).show(top, "choice");
		});
		mi.setShortcut(new MenuShortcut('Z'));
		mi.setEnabled(false);
		
		errorLabel.setFont(new Font(null, Font.BOLD, 20));
		errorLabel.setBackground(Color.DARK_GRAY);
		errorLabel.setAlignment(Label.CENTER);
		errorLabel.setForeground(Color.GREEN);
		
		actions.setEnabled(false);
		sales.setEnabled(false);
		purchases.setEnabled(false);
	}
	
	// Konstruktor
	public StockExchange() {
		super("Stock Exchange");
		setExtendedState(Frame.MAXIMIZED_BOTH);
		
		top.add(choicePanel(), "choice");
		top.add(registerPanel(), "register");
		top.add(loginPanel(), "login");
		top.add(mainPanel(), "main");
		
		add(top);
		add(errorLabel, BorderLayout.SOUTH);
		
		addMenu();
		
		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				dispose();
			}
		});
		
		setVisible(true);
	}
	
	// Main
	public static void main(String[] args) {
		
		System.loadLibrary("TradingViewCpp");
		
		new StockExchange();
	}
}
