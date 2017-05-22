#!/usr/bin/env bash

# download and install stoke to /home/vagrant/stoke
# which is synced to the host's $THIS_PROJECT'S_DIR/stoke
if [ ! -d stoke ]
then
  git clone https://github.com/StanfordPL/stoke
  cd stoke
  ./configure.sh
  make
  echo 'export PATH="$PATH:/home/vagrant/stoke/bin"' >> /home/vagrant/.bashrc
  cd ..
fi
