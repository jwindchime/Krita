rm -rf ~/dev/krita.app/
rm -rf ~/dev/krita.dmg
cp -r ~/dev/i/bin/krita.app ~/dev
cp -r ~/dev/i/share ~/dev/krita.app/Contents/
mkdir -p ~/dev/krita.app/Contents/PlugIns/krita
install_name_tool -add_rpath /Users/boud/dev/i/lib ~/dev/krita.app/Contents/MacOS/krita
~/dev/i/bin/macdeployqt ~/dev/krita.app \
    -verbose=3 \
    -executable=/Users/boud/dev/krita.app/Contents/MacOS/krita \
    -extra-plugins=/Users/boud/dev/i/lib/krita/ \
    -extra-plugins=/Users/boud/dev/i/lib/plugins/ \
    -extra-plugins=/Users/boud/dev/i/plugins/ 
mv ~/dev/krita.app/Contents/PlugIns/krita  ~/dev/krita.app/Contents/
mv ~/dev/krita.app/Contents/PlugIns/kf5  ~/dev/krita.app/Contents/
install_name_tool -delete_rpath  @loader_path/../../../../lib ~/dev/krita.app/Contents/MacOS/krita
install_name_tool -delete_rpath  /Users/boud/dev/i/lib ~/dev/krita.app/Contents/MacOS/krita

