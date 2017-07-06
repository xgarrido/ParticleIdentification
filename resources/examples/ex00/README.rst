ParticleIdentification Falaise module
=====================================

Introduction
------------

This example shows how to use the PID module with ``flreconstruct`` and with
``cuts`` library. A pipeline is defined that processes the SuperNEMO simulated
data (SD bank from the flsimulate output) to produce mock calibration data (CD
bank) then the mock tracker clusterizer (TCD bank), the trackfit fitter (TTD
bank), the charged particle tracking (PTD bank) coupled to the gamma
tracko-clustering and finally, the particle identification algorithms (TD
bank). A last module is added to store events that look like 2 internal
electrons into a dedicated file.

Configuration file(s):

* ``flreconstruct.conf`` : the ``flreconstruct`` pipeline configuration files
  that describes the chain of modules to be processed.

* ``pipeline_modules.def`` : the definition of the pipeline.

* ``service_manager.conf`` : the list of the files defining services.

* ``services.conf`` : the configuration of the services namely the ``geometry``
  service and the ``cuts`` service.

* ``cut_manager.conf`` : the list of the files defining cuts.

* ``cuts/pid_cuts.conf`` : the definition of the particle such as an electron,
  an alpha, ...

* ``cuts/topology_cuts.conf`` : the definition of the channel cuts such as 2
  internal electrons, ...


Usage
-----

  1. First setup the Falaise software.

  2. Generate a set of simulated events with ``flsimulate`` program.

  3. Run the reconstruction pipeline:

      * Browse the manual for the particle identification and the topology
        module. For instance, to retrieve ``particle_identification_module``
        configuration parameters, do the following: ::

          $ bxocd_manual \
            --load-dll Falaise@${FALAISE_INSTALL_DIR}/lib \
            --load-dll Falaise_ParticleIdentification@${FALAISE_INSTALL_DIR}/lib/Falaise/modules \
            --action show snemo::reconstruction::particle_identification_module > falaise_pid.rst
          $ pandoc -w html falaise_pid.rst -o falaise_pid.html
	  $ xdg-open falaise_pid.html &

      * Run the reconstruction pipeline: ::

          $ flreconstruct \
            -p flreconstruct.conf \
            -i your_simulated_file.brio

        The ``flreconstruct`` program should perform the pipeline as defined in
        ``pipeline_modules.def``. At the end, one should get an
        ``io_output_2e_channel_selected.brio`` file that only contains 2
        internal electrons following ``2e::channel_cut`` cut definition from
        ``cuts/topology_modules.conf``. For instance, the definition currently
        select 2 electrons topology (i.e. ``topology_2e_pattern`` data model)
        with an internal probability greater than 4% and an external probability
        smaller than 1%. Moreover, the definition of what an electron is, is up
        to the user and can be found, for this example, in the file
        ``cuts/pid_cuts.conf`` (under the name ``electron_definition``). So far,
        an electron is a particle with a negative electric charge + a
        calorimeter associated and a vertex on the source foil.
