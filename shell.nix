let
	pkgs = import <nixpkgs> {};
in pkgs.mkShell {
	packages = with pkgs; [
		gnumake
		edl
		pkgsCross.aarch64-embedded.buildPackages.gcc
		pkgsCross.aarch64-embedded.buildPackages.binutils
		dtc
		android-tools
	];
}
