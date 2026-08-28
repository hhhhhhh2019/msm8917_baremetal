{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
  flake-utils.lib.eachDefaultSystem(system: let
    pkgs = import nixpkgs { inherit system; config.allowUnfree = true; };

    cc = pkgs.pkgsCross.aarch64-embedded.buildPackages.gcc;
    binutils = pkgs.pkgsCross.aarch64-embedded.buildPackages.binutils;
    python = pkgs.python3.withPackages (ps: [
      ps.kconfiglib
    ]);

    bootloader = pkgs.stdenv.mkDerivation {
      name = "bootloader";
      version = "0.0.1";

      buildInputs = with pkgs; [
        cc
        binutils
        gnumake
        gzip
        python
      ];

      nativeBuildInputs = with pkgs; [
        android-tools
        edl
      ];
    };
  in {
    packages = {};

    devShells = {
      bootloader = pkgs.mkShell {
        inputsFrom = [ bootloader ];

        packages = with pkgs; [
          clang-tools
        ];
      };
    };
  });
}
