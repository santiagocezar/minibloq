let
    pkgs = import <nixpkgs> {};
in
    pkgs.mkShell {
        buildInputs = with pkgs; [
            meson
            ninja
            pkg-config
            libusb-compat-0_1
            wxGTK31

            gdb
        ];
    }
