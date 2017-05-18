# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "bento/ubuntu-16.04"
  host = RbConfig::CONFIG['host_os']
  # Give VM 1/4 system memory & access to all cpu cores on the host
  if host =~ /darwin/
    cpus = `sysctl -n hw.ncpu`.to_i / 2
    # sysctl returns Bytes and we need to convert to MB
    mem = `sysctl -n hw.memsize`.to_i / 1024 / 1024 / 4
  elsif host =~ /linux/
    cpus = `nproc`.to_i / 2
    # meminfo shows KB and we need to convert to MB
    mem = `grep 'MemTotal' /proc/meminfo | sed -e 's/MemTotal://' -e 's/ kB//'`.to_i / 1024 / 4
  else
    cpus = `wmic cpu get NumberOfCores`.split("\n")[2].to_i / 2
    mem = `wmic OS get TotalVisibleMemorySize`.split("\n")[2].to_i / 1024 /4
  end
  config.vm.provider "virtualbox" do |v|
    v.memory = mem
    v.cpus = cpus
  end
  config.vm.provision "shell", path: "sources/install_deps.sh"
  # config.vm.provision "shell", path: "sources/bootstrap.sh", privileged: false
  config.vm.synced_folder ".", "/home/vagrant/minisana"
end
