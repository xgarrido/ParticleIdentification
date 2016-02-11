ParticleIdentification Falaise module
=====================================

Introduction
------------

This example shows how to build a ``flreconstruct`` pipeline that processes the
SuperNEMO simulated data (SD bank from the flsimulate output) to produce mock
calibration data (CD bank) then the mock tracker clusterizer (TCD bank), the
trackfit fitter (TTD bank), the charged particle tracking (PTD bank) coupled to
the gamma tracking and finally, the particle identification algorithms (TD
bank). The latter refers to the particle identification module that tag particle
track given a set of selections such as electric charge, vertex
positions.... The topology module exploits this tag information to build
topology patterns like '2e', '1e', '1eNg'.

Configuration file(s):

  * ``config/pipeline.conf`` : the ``flreconstruct`` pipeline configuration
    files that describes the chain of modules to be processed.

Usage
-----

  1. First setup the Falaise software.

  2. Check the geometry of the SuperNEMO demonstrator (not mandatory): ::

      $ bxgeomtools_inspector \
          --datatools::logging "warning" \
          --load-dll Falaise@${FALAISE_INSTALL_DIR}/lib/x86_64-linux-gnu \
          --datatools::resource_path "falaise@${FALAISE_INSTALL_DIR}/share/Falaise-2.0.0/resources" \
          --manager-config "@falaise:config/snemo/demonstrator/geometry/3.0/manager.conf"

  3. Generate a set of simulated events: ::

      $ flsimulate \
        --experiment "demonstrator" \
        --vertex-generator "source_strips_bulk" \
        --event-generator  "Se82.0nubb" \
	--number 10 \
	--output-file "Se82_0nubb-source_strips_bulk_SD.brio"

  4. Run the reconstruction pipeline:

      * Browse the manual for the particle identification and the topology
        module. For instance, to retrieve ``particle_identification_module``
        configuration parameters, do the following: ::

          $ bxocd_manual \
            --load-dll Falaise@${FALAISE_INSTALL_DIR}/lib/x86_64-linux-gnu \
            --load-dll Falaise_ParticleIdentification@${FALAISE_INSTALL_DIR}/lib/x86_64-linux-gnu/Falaise/modules \
            --action show snemo::reconstruction::particle_identification_module > falaise_pid.rst
          $ pandoc -w html falaise_pid.rst -o falaise_pid.html
	  $ xdg-open falaise_pid.html &


      * Run the reconstruction pipeline: ::

          $ flreconstruct \
            --input-file "Se82_0nubb-source_strips_bulk_SD.brio" \
	    --pipeline "config/pipeline.conf" \
            --output-file "Se82_0nubb-source_strips_bulk_SD-CD-TCD-TTD-PTD-TD.brio"

        The ``flreconstruct`` executable should perform the mock
        pre-reconstruction and then print the structure of the processed event
        records in the terminal. You should be able to see the ``"SD"``,
        ``"CD"``, ``"TCD"``,  ``"TTD"``, ``"PTD"`` and ``"TD"`` banks.

        The output file ``Se82_0nubb-source_strips_bulk_SD-CD-TCD-TTD-PTD-TD.brio``
        is thus ready for further reconstruction, cuts or analysis.
