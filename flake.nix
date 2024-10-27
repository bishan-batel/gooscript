{
  description = "A Nix-flake-based C/C++ development environment";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { 
    self, nixpkgs, flake-utils, ...
    }: flake-utils.lib.eachDefaultSystem (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      libPath = with pkgs; lib.makeLibraryPath [ libxkbcommon ]; 
    in {

      devShells.default = pkgs.mkShell.override {
        stdenv = pkgs.clang19Stdenv;
      } {
          name = "gooscript";
          packages = with pkgs; [
            ninja
            python3
            cmake
            unzip
          ];

          buildInputs = with pkgs; [
            llvmPackages_19.lldb 
            llvmPackages_19.clang-tools
            llvmPackages_19.clang
            pkg-config
          ];

          nativeBuildInputs = with pkgs; [ ];

          LD_LIBRARY_PATH = libPath;
        };

    });
}
