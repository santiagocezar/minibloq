let
    pkgs = import <nixpkgs> {};
    lager = import (builtins.fetchGit {
        url = "https://github.com/arximboldi/lager";
        rev = "503ea6accc0ce6af683a207a9a02de4a9bf0bedc";
    });
in
    pkgs.stdenv.mkDerivation {
        name = "minibloq";

        src = ./.;

        nativeBuildInputs = with pkgs; [
            meson
            ninja
            pkg-config
        ];

        buildInputs = with pkgs; [
            lager
            libusb-compat-0_1
            wxGTK31
        ];
    }
