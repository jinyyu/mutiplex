Name:    libreactor
Version: 0.1
Release: %{?dist}
Summary: libreactor
URL:     https://github.com/jinyyu/libreactor
License: GPLv3+

%description

%prep

%build
cd libreactor
mkdir -p build
cd build
cmake3 .. -DCMAKE_INSTALL_PREFIX=/usr


%make_build



%install
cd libreactor/build
%make_install


%files
/usr/include/libreactor/*
/usr/lib/libreactor.a

%changelog
