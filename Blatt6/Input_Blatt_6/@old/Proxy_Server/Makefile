# Makefile fuer Java-Proxy
# 19/Nov/2018, H.-J. Eikerling

# Parametrisierung des Proxies bei Start
PROXY_AVRG_DELAY=1000
PROXY_SEED=1

# Compiler Einstellungen und Run-Konfiguration
PKG=$(TOPPKGDIR)/hsos/bs
TOPPKGDIR=de
MAINCLASS=ProxyServer

ALL=$(PKG)/$(MAINCLASS).class

JAVAC=javac
JAVA=java

help: 
	@echo Optionen fuer 'make':
	@echo "   -" Kompilieren von $(MAINCLASS).java mit \'make all\'
	@echo "   -" Starten des Proxies mit \'make run\'
	@echo "   -" Loeschen der generierten Klassen mit \'make clean\'

all: $(ALL)

$(PKG)/$(MAINCLASS).class: $(MAINCLASS).java
	$(JAVAC) -d . $(MAINCLASS).java

run: $(PKG)/$(MAINCLASS).class
	$(JAVA) $(PKG)/$(MAINCLASS) $(PROXY_AVRG_DELAY) $(PROXY_SEED)

clean:
	$(RM) -rf $(TOPPKGDIR)
