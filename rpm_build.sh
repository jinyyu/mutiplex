set -e

rm -rf build
rm -rf ~/rpmbuild/BUILD/libreactor
cd ../
cp -r libreactor ~/rpmbuild/BUILD

cd libreactor
rpmbuild -bb reactor.spec