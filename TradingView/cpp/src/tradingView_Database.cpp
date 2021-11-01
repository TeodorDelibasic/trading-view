#include "tradingView_Database.h"
#include "database.h"

JNIEXPORT void JNICALL Java_tradingView_Database_createConnection
(JNIEnv*, jclass) {
    Database& db = Database::getInstance();
}

/*
 * Class:     tradingView_Database
 * Method:    checkUser
 * Signature: (LtradingView/User;)Z
 */
JNIEXPORT jboolean JNICALL Java_tradingView_Database_checkUser
(JNIEnv* env, jobject, jobject user) {
    jclass cls = env->GetObjectClass(user);

    jmethodID mid1 = env->GetMethodID(cls, "getUsername", "()Ljava/lang/String;");
    if (mid1 == 0) {
        return false;
    }
    jstring username = jstring(env->CallObjectMethod(user, mid1));

    jmethodID mid2 = env->GetMethodID(cls, "getPassword", "()Ljava/lang/String;");
    if (mid2 == 0) {
        return false;
    }
    jstring password = jstring(env->CallObjectMethod(user, mid2));

    jmethodID mid3 = env->GetMethodID(cls, "getBalance", "()D");
    if (mid3 == 0) {
        return false;
    }
    jdouble balance = env->CallDoubleMethod(user, mid3);

    const char* usernameReturn = env->GetStringUTFChars(username, 0);
    const char* passwordReturn = env->GetStringUTFChars(password, 0);

    User u(usernameReturn, passwordReturn, balance);

    env->ReleaseStringUTFChars(username, usernameReturn);
    env->ReleaseStringUTFChars(password, passwordReturn);

    return Database::getInstance().checkUser(u);
}

/*
 * Class:     tradingView_Database
 * Method:    checkUsername
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_tradingView_Database_checkUsername
(JNIEnv* env, jobject, jstring username) {
    const char* usernameReturn = env->GetStringUTFChars(username, 0);
    jboolean check = Database::getInstance().checkUsername(usernameReturn);
    env->ReleaseStringUTFChars(username, usernameReturn);
    return check;
}

/*
 * Class:     tradingView_Database
 * Method:    insertUser
 * Signature: (LtradingView/User;)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_insertUser
(JNIEnv* env, jobject, jobject user) {
    jclass cls = env->GetObjectClass(user);

    jmethodID mid1 = env->GetMethodID(cls, "getUsername", "()Ljava/lang/String;");
    if (mid1 == 0) {
        return;
    }
    jstring username = jstring(env->CallObjectMethod(user, mid1));

    jmethodID mid2 = env->GetMethodID(cls, "getPassword", "()Ljava/lang/String;");
    if (mid2 == 0) {
        return;
    }
    jstring password = jstring(env->CallObjectMethod(user, mid2));

    jmethodID mid3 = env->GetMethodID(cls, "getBalance", "()D");
    if (mid3 == 0) {
        return;
    }
    jdouble balance = env->CallDoubleMethod(user, mid3);

    const char* usernameReturn = env->GetStringUTFChars(username, 0);
    const char* passwordReturn = env->GetStringUTFChars(password, 0);

    User u(usernameReturn, passwordReturn, balance);

    env->ReleaseStringUTFChars(username, usernameReturn);
    env->ReleaseStringUTFChars(password, passwordReturn);

    Database::getInstance().insertUser(u);
}

/*
 * Class:     tradingView_Database
 * Method:    getUserId
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_tradingView_Database_getUserId
(JNIEnv* env, jobject, jstring username) {
    const char* usernameReturn = env->GetStringUTFChars(username, 0);
    jint id = Database::getInstance().getUserId(usernameReturn);
    env->ReleaseStringUTFChars(username, usernameReturn);
    return id;
}

/*
 * Class:     tradingView_Database
 * Method:    getStockId
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_tradingView_Database_getStockId
(JNIEnv* env, jobject, jstring symbol) {
    const char* symbolReturn = env->GetStringUTFChars(symbol, 0);
    jint id = Database::getInstance().getStockId(symbolReturn);
    env->ReleaseStringUTFChars(symbol, symbolReturn);
    return id;
}

/*
 * Class:     tradingView_Database
 * Method:    checkIfStockExists
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_tradingView_Database_checkIfStockExists
(JNIEnv* env, jobject, jstring symbol) {
    const char* symbolReturn = env->GetStringUTFChars(symbol, 0);
    boolean check = Database::getInstance().checkIfStockExists(symbolReturn);
    env->ReleaseStringUTFChars(symbol, symbolReturn);
    return check;
}

/*
 * Class:     tradingView_Database
 * Method:    addStock
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_addStock
(JNIEnv* env, jobject, jstring symbol) {
    const char* symbolReturn = env->GetStringUTFChars(symbol, 0);
    Database::getInstance().addStock(symbolReturn);
    env->ReleaseStringUTFChars(symbol, symbolReturn);
}

/*
 * Class:     tradingView_Database
 * Method:    checkIfOwnsExists
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_tradingView_Database_checkIfOwnsExists
(JNIEnv* env, jobject, jint idUser, jint idStock) {
    return Database::getInstance().checkIfOwnsExists(idUser, idStock);
}

/*
 * Class:     tradingView_Database
 * Method:    addOwnsStock
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_addOwnsStock
(JNIEnv*, jobject, jint idUser, jint idStock) {
    Database::getInstance().addOwnsStock(idUser, idStock);
}

/*
 * Class:     tradingView_Database
 * Method:    insertBuying
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_insertBuying
(JNIEnv*, jobject, jint idTrans, jint amount) {
    Database::getInstance().insertBuying(idTrans, amount);
}

/*
 * Class:     tradingView_Database
 * Method:    getSymbol
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tradingView_Database_getSymbol
(JNIEnv* env, jobject, jint idTrans) {
    std::string str = Database::getInstance().getSymbol(idTrans);
    return (env)->NewStringUTF(str.c_str());
}

/*
 * Class:     tradingView_Database
 * Method:    checkHas
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_tradingView_Database_checkHas
(JNIEnv*, jobject, jint idBought, jint idAmount) {
    return Database::getInstance().checkHas(idBought, idAmount);
}

/*
 * Class:     tradingView_Database
 * Method:    getProfit
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_tradingView_Database_getProfit
(JNIEnv*, jobject, jint idBought, jdouble price) {
    return Database::getInstance().getProfit(idBought, price);
}

/*
 * Class:     tradingView_Database
 * Method:    updateBuying
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_updateBuying
(JNIEnv*, jobject, jint idBought, jint amount) {
    Database::getInstance().updateBuying(idBought, amount);
}

/*
 * Class:     tradingView_Database
 * Method:    checkIfNotSold
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_tradingView_Database_checkIfNotSold
(JNIEnv*, jobject, jint idUser, jint idStock) {
    return Database::getInstance().checkIfNotSold(idUser, idStock);
}

/*
 * Class:     tradingView_Database
 * Method:    removeOwnsStock
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_removeOwnsStock
(JNIEnv*, jobject, jint idUser, jint idStock) {
    Database::getInstance().removeOwnsStock(idUser, idStock);
}

/*
 * Class:     tradingView_Database
 * Method:    insertSelling
 * Signature: (IDI)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_insertSelling
(JNIEnv*, jobject, jint idTrans, jdouble profit, jint idBought) {
    Database::getInstance().insertSelling(idTrans, profit, idBought);
}

/*
 * Class:     tradingView_Database
 * Method:    lastTransId
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_tradingView_Database_lastTransId
(JNIEnv*, jobject) {
    return Database::getInstance().lastTransId();
}

/*
 * Class:     tradingView_Database
 * Method:    insertTransaction
 * Signature: (DIII)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_insertTransaction
(JNIEnv*, jobject, jdouble price, jint amount, jint idUser, jint idStock) {
    Database::getInstance().insertTransaction(price, amount, idUser, idStock);
}

/*
 * Class:     tradingView_Database
 * Method:    updateBalance
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL Java_tradingView_Database_updateBalance
(JNIEnv*, jobject, jint idUser, jdouble balance) {
    Database::getInstance().updateBalance(idUser, balance);
}

/*
*Class:     tradingView_Database
* Method : getBalance
* Signature : (I)D
*/
JNIEXPORT jdouble JNICALL Java_tradingView_Database_getBalance
(JNIEnv*, jobject, jint idUser) {
    return Database::getInstance().getBalance(idUser);
}

/*
 * Class:     tradingView_Database
 * Method:    getOwnedStocks
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tradingView_Database_getOwnedStocks
(JNIEnv* env, jobject, jint idUser) {
    std::string str = Database::getInstance().getOwnedStocks(idUser);
    return (env)->NewStringUTF(str.c_str());
}

/*
 * Class:     tradingView_Database
 * Method:    getBought
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tradingView_Database_getBought
(JNIEnv* env, jobject, jint idUser) {
    std::string str = Database::getInstance().getBought(idUser);
    return (env)->NewStringUTF(str.c_str());
}

/*
 * Class:     tradingView_Database
 * Method:    getSold
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_tradingView_Database_getSold
(JNIEnv* env, jobject, jint idUser) {
    std::string str = Database::getInstance().getSold(idUser);
    return (env)->NewStringUTF(str.c_str());
}