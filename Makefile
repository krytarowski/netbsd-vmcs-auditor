#	$NetBSD: Makefile,v 1.7 2010/02/03 15:34:38 roy Exp $

PROG_CXX=	vmcs-auditor
NOMAN=		#
SRCS=		Auditor.cpp VmcsAuditor.cpp

.include <bsd.prog.mk>
