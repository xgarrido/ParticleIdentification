#!/usr/bin/env python
import os
import argparse
import subprocess
from subprocess import check_output

def get_configuration_dir():
    """ Derive path to resource dir from this
    """
    this_dir = os.path.realpath(__file__)
    return os.path.join(os.path.dirname(this_dir), "config/")

def get_bayeux_library_dir():
    """ Return Bayeux library directory from bxquery binary
    """
    return check_output(["bxquery", "--libdir"]).decode().rstrip()

def get_falaise_library_dir():
    """ Return Falaise library directory from flquery binary
    """
    return check_output(["flquery", "--libdir"]).decode().rstrip()

def get_falaise_resource_dir():
    """ Return Falaise resource directory from flquery binary
    """
    return check_output(["flquery", "--resourcedir"]).decode().rstrip()

def main():
    """ Implementation of particle and topology identification module
    """
    parser = argparse.ArgumentParser(description="Run Falaise pipeline with PID support")
    parser.add_argument("-i", type=str, metavar="<infile>", help="input BRIO file")
    parser.add_argument("-o", type=str, metavar="<outfile>", help="output ROOT file")
    parser.add_argument("-n", type=int, metavar="<number>",
                        required=True, help="set number of simulated events")
    parser.add_argument("-m", "--module", type=str, metavar="<name>", default="pipeline",
                        help="set the module name")
    args = parser.parse_args()

    try:
        process = [
            "bxdpp_processing",
            "--dlls-config", get_configuration_dir() + "dlls.conf",
            "--max-records", str(args.n),
            "--module-manager-config", get_configuration_dir() + "module_manager.conf",
            "--module", args.module,
            "--datatools::resource-path=falaise@" + get_falaise_resource_dir(),
            "--datatools::resource-path=bayeux_lib_dir@" + get_bayeux_library_dir(),
            "--datatools::resource-path=falaise_lib_dir@" + get_falaise_library_dir(),
            "--datatools::resource-path=configuration@" + get_configuration_dir()
        ]
        if args.i:
            process.append("-i " + args.i)
        if args.o:
            process.append("-o " + args.o)
        subprocess.call(process)
    except OSError as e:
        if e.errno == os.errno.ENOENT:
            print("error: could not locate bxdpp_processing program")
        else:
            # Something else went wrong while trying to run `bxdpp_processing`
            raise

if __name__ == '__main__':
    main()
