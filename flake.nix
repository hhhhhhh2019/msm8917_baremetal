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

	bootloader = pkgs.stdenv.mkDerivation {
	  pname = "bootloader";
	  version = "0.1.0";

	  src = pkgs.lib.fileset.toSource { root = ./.; fileset = pkgs.lib.fileset.unions [ ./bootloader ./Makefile.defaults ./pmos.dtb ]; };

	  nativeBuildInputs = with pkgs; [
            gnumake
            pkgsCross.aarch64-embedded.buildPackages.gcc
            pkgsCross.aarch64-embedded.buildPackages.binutils
            dtc
            android-tools
	  ];

	  buildPhase = ''
	    cd bootloader
	    make
	  '';

	  installPhase = ''
	    mkdir -p $out
            cp build/boot.img $out/
            cp build/bootloader.elf.map $out/
	  '';
	};
      in {
 	packages = {
	  inherit bootloader;
	};

        devShell = pkgs.mkShell {
	  inputsFrom = [ bootloader ];
          packages = with pkgs; [
            edl
          ];
        };
      }
    );
}
