# -*- mode: ruby -*-
# vi: set ft=ruby :
Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/yakkety64"

  config.vm.network "forwarded_port", guest: 3000, host: 3000

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get upgrade -y
    apt-get install -y zsh build-essential g++ byobu git postgresql libpq-dev libpqxx-4.0v5 libpqxx-dev vim-nox
    chsh -s /bin/zsh ubuntu
  SHELL
end
