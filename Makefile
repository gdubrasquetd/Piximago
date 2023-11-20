DOMAIN_DIR = Domain/src
INTERFACE_DIR = Interface

all: domain interface shortcut

domain:
	cd $(DOMAIN_DIR) && $(MAKE)

interface:
	cd $(INTERFACE_DIR) && qmake
	cd $(INTERFACE_DIR) && $(MAKE)

clean:
	cd $(DOMAIN_DIR) && $(MAKE) clean
	cd $(INTERFACE_DIR) && $(MAKE) clean
	if [ -e ./Piximago-shortcut ]; then rm ./Piximago-shortcut; fi

shortcut:
	if [ -e ./Piximago-shortcut ]; then rm ./Piximago-shortcut; fi
	ln -s ./build/Piximago ./Piximago-shortcut
