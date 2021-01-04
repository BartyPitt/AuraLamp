#define TaskChanging 
#define MainDebug
#define Ticker2

#ifdef TaskChanging
    #define TaskChangingPrint(X) Serial.println(X)
#else
    #define TaskChangingPrint(X)
#endif

#ifdef MainDebug
    #define MainDebugPrint(X) Serial.println(X)
#else
    #define MainDebugPrint(X)
#endif

#ifdef Ticker2
    #define TickerPrint(X) Serial.println(X)
#else
    #define TickerPrint(X)
#endif
