ver = 3.1.66
os = windows
arch = $(strip $(shell python3 ../arch.py))
fs_uae_dir = FS-UAE/Windows/$(arch)
fs_uae_zip = fs-uae_$(ver)_$(os)_$(arch).zip
fs_uae_txz = fs-uae_$(ver)_$(os)_$(arch).tar.zx

all: clean progdir zip

clean:
	rm -Rf fs-uae-*_windows*
	rm -Rf fs-uae_*_windows*
	rm -Rf FS-UAE

progdir:
	rm -Rf FS-UAE
	mkdir -p $(fs_uae_dir)

	make -C ../..

        # For qemu-uae on Windows
	# python3 clib.py libpixman-1-0.dll $(fs_uae_dir)

	cp -a ../../fs-uae.exe $(fs_uae_dir)/
	cp -a ../../fs-uae-device-helper.exe $(fs_uae_dir)/

	# mkdir -p FS-UAE/Data
	# cp -a ../../fs-uae.dat FS-UAE/Data
	cp -a ../../fs-uae.dat $(fs_uae_dir)/
	mkdir -p FS-UAE/Locale
	cp -a ../../share/locale/* FS-UAE/Locale

	cp -a ../../licenses FS-UAE/Licenses
	cp -a ../../README FS-UAE/ReadMe.txt

	echo $(ver) > FS-UAE/Version.txt
	echo $(ver) > $(fs_uae_dir)/Version.txt

	# strip $(fs_uae_dir)/*.exe
	# strip $(fs_uae_dir)/*.dll

	cd ../.. && python3 fsbuild/standalone.py dist/windows/$(fs_uae_dir)

	python3 sign.py $(fs_uae_dir)/*.exe

zip:
	# cd $(fs_uae_dir) && zip -9 -r ../../../$(fs_uae_dir).zip *
	rm -f ../../$(fs_uae_zip)
	zip -9 -r ../../$(fs_uae_zip) FS-UAE

tar.xz: progdir
	# cd $(fs_uae_dir) && tar cfvJ ../../../$(fs_uae_dir).tar.xz *
	tar cfvJ ../../$(fs_uae_txz) FS-UAE

setup:
	python3 iscc.py fs-uae.iss
	python3 sign.py ../../$(fs_uae_dir).exe
