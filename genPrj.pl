#!/usr/bin/perl
 
use strict;
use warnings;
use Getopt::Long;
use File::Copy;
use Cwd;

sub generate_target
{
	my @vars = (@_);

	mkdir "build";
	chdir "build";

	system("cmake -G $vars[1] -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON -DCMAKE_TOOLCHAIN_FILE=../$vars[0]ToolChain.txt \"-DBinaries=$vars[4]\" -DMode:STRING=$vars[3] -DSolutionName=$vars[2] -DOptMode:STRING=$vars[5] -DCMAKE_INSTALL_PREFIX:STRING=$vars[6] -DSYSTEM_ARCHITECTURE:STRING=$vars[7] -DCMAKE_CUDA_AVAILABLE=$vars[8] VERBOSE=1 ..");

	chdir "..";
}

sub showHelp
{
	print "NAME\n";
	print "genPrj.pl \n";
	print "\n";
	print "SYNOPSIS\n";
	print "\tProject file generator.\n";
	print "\n";
	print "SYNTAX\n";
	print "\tFor its proper execution, you shall execute in a terminal: \n";
	print "\n";
	print "\t\t perl genPrj.pl [[--project] <String>] [[--platform] <String>] [[--cuda] <String>]\n";
	print "\n";
	print "DESCRIPTION\n";
	print "\tThis program is devoted to the project file generation of a given cmake structure.\n";
	print "\n";
	print "PARAMETERS\n";
	print "\t--project\n";
	print "\t\tSpecifies the project to be generated.\n";
	print "\n";
	print "\t\tRequires \t\t true";
	print "\n";
	print "\t--platform\n";
	print "\t\tSpecifies the platform in which the project will be compiled.\n";
	print "\n";
	print "\t\tRequires \t\t true";
	print "\n";
	print "\t--cuda\n";
	print "\t\tSpecifies wether we want to use cuda capabilities or not.\n";
	print "\n";
	print "\t\tRequires \t\t false";
	print "\n";
	print "\t\tAccepted values \t On/Off";
	print "\n";
	print "\t\tDefault value \t\t On";
	print "\n";

}

my $project = '';	# option variable with default value
my $platform = 'Windows';
if($^O eq 'linux')
{
	$platform = 'Linux';
}
elsif($^O eq "darwin")
{
	$platform = 'iOS';
}
elsif($^O eq "MSWin32")
{
	$platform = 'Windows';
}

my $Compiler = "\"Visual Studio 16 2019\"";
my $SolutionName = '';
my $Mode='';
my $Binaries='';
my $optmode='Release';
my $installdir=getcwd . '/bin';
my $architecture= 'x86-64';
my $cudaCapabilities='On';

GetOptions ('project=s' => \$project, 'platform=s' => \$platform, 'Mode=s' => \$optmode, 'install_dir=s' => \$installdir, 'arch=s' => \$architecture, 'cuda=s' => \$cudaCapabilities);

if($project eq "Windows")
{
	$Mode='ALL_Windows';
	$Binaries='XR-MCU';
	$SolutionName='XR-MCU';
}
elsif($project eq "ALL_Linux")
{
	$Mode='ALL_Linux';
	$Binaries='XR-MCU';
	$SolutionName='XR-MCU';
}
elsif($project eq "iOS")
{
	$Mode='ALL_iOS';
	$Binaries='XR-MCU';
	$SolutionName='XR-MCU';
}
else
{
	showHelp();
	
	exit -1;
}

if($cudaCapabilities ne "On" and $cudaCapabilities ne "Off")
{
	showHelp();
	
	exit -1;
}

if($platform eq "Windows")
{
	generate_target('win64',$Compiler,$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture,$cudaCapabilities);
}
elsif($platform eq "Linux")
{
	generate_target('linux64',"\"CodeBlocks - Unix Makefiles\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture,$cudaCapabilities);
}
elsif($platform eq "iOS")
{
	generate_target('iOS',"\"Xcode\"",$SolutionName,$Mode,$Binaries,$optmode,$installdir,$architecture,$cudaCapabilities);
}
else
{
	showHelp();
	
	exit -1;
}

exit 0;
