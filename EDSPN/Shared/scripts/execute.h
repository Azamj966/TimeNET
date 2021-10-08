/*===============> NEEDED BY count.script */

/******************************************************************************
Makros, die bei der Uebersetzung des von programOutput.c automatisch
generierten Programms benoetigt werden
*******************************************************************************/

/* quotient in euclidean division */

#define div(a,b) ((a) >= 0 ? (a)/(b) : -((-(a)+(b)-1)/(b)))

/* some n-ary min and max */

#define min2(a,b) ((a) <= (b) ? (a) : (b))
#define max2(a,b) ((a) >= (b) ? (a) : (b))

#define max3(a,b,c) (max2((a),max2((b),(c))))
#define min3(a,b,c) (min2((a),min2((b),(c))))

#define max4(a,b,c,d) (max2((a),max3((b),(c),(d))))
#define min4(a,b,c,d) (min2((a),min3((b),(c),(d))))

#define max5(a1,a2,a3,a4,a5) (max2(max4((a1),(a2),(a3),(a4)),(a5)))
#define min5(a1,a2,a3,a4,a5) (min2(min4((a1),(a2),(a3),(a4)),(a5)))

#define max6(a1,a2,a3,a4,a5,a6) (max2(max5((a1),(a2),(a3),(a4),(a5)),(a6)))
#define min6(a1,a2,a3,a4,a5,a6) (min2(min5((a1),(a2),(a3),(a4),(a5)),(a6)))

#define max7(a1,a2,a3,a4,a5,a6,a7) (max2(max6((a1),(a2),(a3),(a4),(a5),(a6)),(a7)))
#define min7(a1,a2,a3,a4,a5,a6,a7) (min2(min6((a1),(a2),(a3),(a4),(a5),(a6)),(a7)))

#define max8(a1,a2,a3,a4,a5,a6,a7,a8) (max2 (max7((a1),(a2),(a3),(a4),(a5),(a6),(a7)),(a8)))
#define min8(a1,a2,a3,a4,a5,a6,a7,a8) (min2(min7((a1),(a2),(a3),(a4),(a5),(a6),(a7)),(a8)))

#define max9(a1,a2,a3,a4,a5,a6,a7,a8,a9) (max2(max8((a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)),(a9)))
#define min9(a1,a2,a3,a4,a5,a6,a7,a8,a9) (min2(min8((a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)),(a9)))


#define max12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) (max2(max6((a1),(a2),(a3),(a4),(a5),(a6)),max6((a7),(a8),(a9),(a10),(a11),(a12))))
#define min12(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) (min2(min6((a1),(a2),(a3),(a4),(a5),(a6)),min6((a7),(a8),(a9),(a10),(a11),(a12))))

#define max13(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) (max2(max12((a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10),(a11),(a12)),(a13)))
#define min13(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) (min2(min12((a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10),(a11),(a12)),(a13)))
