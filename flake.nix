{
  description = "miniBloq";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      pkgs = nixpkgs.legacyPackages.x86_64-linux;
      inherit (pkgs) stdenv;
    in
      {
        packages.x86_64-linux.default = stdenv.mkDerivation {
          nativeBuildInputs = with pkgs; [
            meson
            ninja
            pkg-config
          ];
          buildInputs = with pkgs; [
            libusb-compat-0_1
            wxGTK31
          ];
        };
        devShell.x86_64-linux = pkgs.mkShell {
          buildInputs = with pkgs; [
              meson
              ninja
              pkg-config
              libusb-compat-0_1
              wxGTK31

              gdb
          ];
        };
      };
}
