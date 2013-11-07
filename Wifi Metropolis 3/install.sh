clear
echo Remove old Wifi Metropolis 3...
sudo rm /opt/Wifi_Metropolis/ -R
echo
echo Extract New Version...
sudo tar -xzvf Wifi_Metropolis.tar.gz -C /opt/
echo
echo Make Shortcut on Desktop ...
sudo cp '/opt/Wifi_Metropolis/Wifi Metropolis 30.desktop' $HOME/Desktop/'Wifi Metropolis 3.desktop'
echo
echo You need Gambas 3.4.1 
echo
echo 'Install Complete!'
exit
