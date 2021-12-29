PROG = proj

SRCS = proj.c timer.c kbd.c mouse.c graphics_card.c utils.c Game.c Button.c Menu.c Piece.c Player.c Chessboard.c Letter.c Digit.c Chess.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic -D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>