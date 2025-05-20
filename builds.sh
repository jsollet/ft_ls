#!/bin/bash

set -e  # Stopper si une commande échoue

INSTALL_DIR="$PWD/local/install"
SRC_DIR="$PWD/local"
ATTR_VERSION="2.5.1"
ACL_VERSION="2.3.1"

mkdir -p "$SRC_DIR" "$INSTALL_DIR"

cd "$SRC_DIR"

# Téléchargement des sources
if [ ! -f "attr-${ATTR_VERSION}.tar.gz" ]; then
  curl -LO "https://download.savannah.nongnu.org/releases/attr/attr-${ATTR_VERSION}.tar.gz"
fi

if [ ! -f "acl-${ACL_VERSION}.tar.gz" ]; then
  curl -LO "https://download.savannah.nongnu.org/releases/acl/acl-${ACL_VERSION}.tar.gz"
fi

# Extraction
tar -xf "attr-${ATTR_VERSION}.tar.gz"
tar -xf "acl-${ACL_VERSION}.tar.gz"

# Compilation attr
cd "attr-${ATTR_VERSION}"
./configure --prefix="$INSTALL_DIR"
make
make install

# Copie manuelle des headers attr manquants (comme error_context.h, etc.)
mkdir -p "$INSTALL_DIR/include/attr"
cp include/attr/*.h "$INSTALL_DIR/include/attr/"

cd "$SRC_DIR"

# Compilation acl
cd "acl-${ACL_VERSION}"
export CPPFLAGS="-I$INSTALL_DIR/include"
export LDFLAGS="-L$INSTALL_DIR/lib"

#./configure --prefix=/project/local/install
./configure --prefix="$INSTALL_DIR" --disable-gettext
make
make install

export PATH="$INSTALL_DIR/bin:$PATH"

echo "✅ Installation terminée dans $INSTALL_DIR"
