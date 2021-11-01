#ifndef _exceptions_h_
#define _exceptions_h_

#include <exception>
#include <string>

// -------------------------- Menu exceptioni ------------------------

class EndOfProgram : public std::exception {

public:
	EndOfProgram()
		: std::exception("Hvala na koriscenju, dovidjenja!!!") { }
};

class IrregularOption : public std::exception {

public:
	IrregularOption()
		: std::exception("Nepostojeca opcija!") { }
};

// ------------------------------- Login -------------------------------

class Register : public std::exception {

public:
	Register()
		: std::exception("Korisnicko ime je zauzeto!") { }
};

class Login : public std::exception {

public:
	Login()
		: std::exception("Pogresno korisnicko ime ili lozinka!") { }
};

// -----------------------------------------------------------------------------

// -------------------------------- Date ---------------------------------------
class IrregularDate : public std::exception {

public:
	IrregularDate()
		: std::exception("Nepravilan format datuma!") { }
};

class IrregularTimestamp : public std::exception {

public:
	IrregularTimestamp()
		: std::exception("Nepravilna vremenska oznaka!") { }
};

class IrregularInterval : public std::exception {

public:
	IrregularInterval()
		: std::exception("Start intervala mora biti pre kraja intervala!") { }
};
// -----------------------------------------------------------------------------

// ---------------------------- Transactions -----------------------------------
class InsufficientFunds : public std::exception {

public:
	InsufficientFunds()
		: std::exception("Nemate dovoljno para!") { }
};

class InsufficientShares : public std::exception {

public:
	InsufficientShares()
		: std::exception("Nemate dovoljno deonica!") { }
};

class TransactionID : public std::exception {

public:
	TransactionID()
		: std::exception("Niste kupovali deonice sa tim ID transakcije!") { }
};
// ------------------------------------------------------------------------------

// ----------------------------- Constructors ------------------------------------
class CandlestickError : public std::exception {

public:
	CandlestickError() 
		: std::exception("Greska: Nisu dozvoljene negativne vrednosti za atribute sveca!!!") { }
};

class UserBalance : public std::exception {

public:
	UserBalance()
		: std::exception("Pocetni iznos na racunu korisnika mora biti veci od nule!") { }
};
//---------------------------------------------

class JsonError : public std::exception {

public:
	JsonError(const std::string& error)
		: std::exception(error.c_str()) { }
};

#endif