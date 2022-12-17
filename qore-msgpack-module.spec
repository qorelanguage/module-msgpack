%{?_datarootdir: %global mydatarootdir %_datarootdir}
%{!?_datarootdir: %global mydatarootdir %{buildroot}/usr/share}

%global module_dir %{_libdir}/qore-modules
%global user_module_dir %{mydatarootdir}/qore-modules/

Name:           qore-msgpack-module
Version:        1.0.1
Release:        1
Summary:        Qorus Integration Engine - Qore msgpack module
License:        MIT
Group:          Development/Languages/Other
Url:            https://qoretechnologies.com
Source:         qore-msgpack-module-%{version}.tar.bz2
BuildRequires:  gcc-c++
%if 0%{?el7}
BuildRequires:  devtoolset-7-gcc-c++
%endif
BuildRequires:  cmake >= 3.5
BuildRequires:  qore >= 1.12.4
BuildRequires:  qore-devel >= 1.12.4
BuildRequires:  qore-stdlib >= 1.12.4
BuildRequires:  doxygen
Requires:       %{_bindir}/env
Requires:       qore >= 1.0
BuildRoot:      %{_tmppath}/%{name}-%{version}-build

%description
This package contains the msgpack module for the Qore Programming Language.

%prep
%setup -q

%build
%if 0%{?el7}
# enable devtoolset7
. /opt/rh/devtoolset-7/enable
%endif
export CXXFLAGS="%{?optflags}"
cmake -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=RELWITHDEBINFO -DCMAKE_SKIP_RPATH=1 -DCMAKE_SKIP_INSTALL_RPATH=1 -DCMAKE_SKIP_BUILD_RPATH=1 -DCMAKE_PREFIX_PATH=${_prefix}/lib64/cmake/Qore .
make %{?_smp_mflags}
make %{?_smp_mflags} docs
sed -i 's/#!\/usr\/bin\/env qore/#!\/usr\/bin\/qore/' test/*.qtest

%install
make DESTDIR=%{buildroot} install %{?_smp_mflags}

%files
%{module_dir}

%check
qore -l ./msgpack-api-1.3.qmod test/msgpack.qtest

%package doc
Summary: Documentation and examples for the Qore msgpack module
Group: Development/Languages/Other

%description doc
This package contains the HTML documentation and example programs for the Qore
msgpack module.

%files doc
%defattr(-,root,root,-)
%doc docs/msgpack test

%changelog
* Sat Dec 17 2022 David Nichols <david@qore.org> 1.0.1
- updated to version 1.0.1
- added tests and doc package to spec file

* Wed Jan 27 2022 David Nichols <david@qore.org> 1.0.0
- initial 1.0.0 release

