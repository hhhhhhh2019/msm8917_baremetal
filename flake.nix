{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable"; # for some reason stable version 25.11 doesn't have cache for gcc
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs { inherit system; config.allowUnfree = true; };
      in {
        devShell = pkgs.mkShell {
          packages = with pkgs; [
            gnumake
            edl
            pkgsCross.aarch64-embedded.buildPackages.gcc
            pkgsCross.aarch64-embedded.buildPackages.binutils
            dtc
            android-tools
          ];
        };
      }
    );
}
