/*
###############################################################################
# If you use PhysiCell in your project, please cite PhysiCell and the version #
# number, such as below:                                                      #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1].    #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# See VERSION.txt or call get_PhysiCell_version() to get the current version  #
#     x.y.z. Call display_citations() to get detailed information on all cite-#
#     able software used in your PhysiCell application.                       #
#                                                                             #
# Because PhysiCell extensively uses BioFVM, we suggest you also cite BioFVM  #
#     as below:                                                               #
#                                                                             #
# We implemented and solved the model using PhysiCell (Version x.y.z) [1],    #
# with BioFVM [2] to solve the transport equations.                           #
#                                                                             #
# [1] A Ghaffarizadeh, R Heiland, SH Friedman, SM Mumenthaler, and P Macklin, #
#     PhysiCell: an Open Source Physics-Based Cell Simulator for Multicellu-  #
#     lar Systems, PLoS Comput. Biol. 14(2): e1005991, 2018                   #
#     DOI: 10.1371/journal.pcbi.1005991                                       #
#                                                                             #
# [2] A Ghaffarizadeh, SH Friedman, and P Macklin, BioFVM: an efficient para- #
#     llelized diffusive transport solver for 3-D biological simulations,     #
#     Bioinformatics 32(8): 1256-8, 2016. DOI: 10.1093/bioinformatics/btv730  #
#                                                                             #
###############################################################################
#                                                                             #
# BSD 3-Clause License (see https://opensource.org/licenses/BSD-3-Clause)     #
#                                                                             #
# Copyright (c) 2015-2018, Paul Macklin and the PhysiCell Project             #
# All rights reserved.                                                        #
#                                                                             #
# Redistribution and use in source and binary forms, with or without          #
# modification, are permitted provided that the following conditions are met: #
#                                                                             #
# 1. Redistributions of source code must retain the above copyright notice,   #
# this list of conditions and the following disclaimer.                       #
#                                                                             #
# 2. Redistributions in binary form must reproduce the above copyright        #
# notice, this list of conditions and the following disclaimer in the         #
# documentation and/or other materials provided with the distribution.        #
#                                                                             #
# 3. Neither the name of the copyright holder nor the names of its            #
# contributors may be used to endorse or promote products derived from this   #
# software without specific prior written permission.                         #
#                                                                             #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  #
# POSSIBILITY OF SUCH DAMAGE.                                                 #
#                                                                             #
###############################################################################
*/

#include "./custom.h"

int oxygen_i, glucose_i; 
int energy_vi; 

// These are for C
//#define STATIC_RRC
// #include "rrc_api.h"
// #include "rrc_types.h"
// #include "rrc_utilities.h"
// extern "C" rrc::RRHandle createRRInstance();

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
// #include <vector>
#include <string>

//rwh - comment out due to reading in from XML
// Cell_Definition fibroblast; 
// Cell_Definition KRAS_positive;
// Cell_Definition KRAS_negative;
// Cell_Definition test; 

void create_cell_types( void )
{
	// use the same random seed so that future experiments have the 
	// same initial histogram of oncoprotein, even if threading means 
	// that future division and other events are still not identical 
	// for all runs 
	
	SeedRandom( parameters.ints("random_seed") ); // or specify a seed here 
	
	// housekeeping 
	
	initialize_default_cell_definition();

  	initialize_cell_definitions_from_pugixml();   //rwh

    //rwh
    cell_defaults.functions.volume_update_function = standard_volume_update_function;
	cell_defaults.functions.update_velocity = standard_update_cell_velocity;

	// cell_defaults.functions.update_migration_bias = weighted_motility_function; 
	cell_defaults.functions.update_migration_bias = NULL; 
	cell_defaults.functions.update_phenotype = NULL; // update_cell_and_death_parameters_O2_based; 
	cell_defaults.functions.custom_cell_rule = NULL; 
	
	cell_defaults.functions.add_cell_basement_membrane_interactions = NULL; 
	cell_defaults.functions.calculate_distance_to_membrane = NULL; 


    /*
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment ); 
	
	// Name the default cell type 
	
	cell_defaults.type = 0; 
	cell_defaults.name = "default cell"; 
	
	// set default cell cycle model 

	cell_defaults.functions.cycle_model = live; 
	
	// set default_cell_functions; 
	
	cell_defaults.functions.update_phenotype = NULL; 
	*/
	// only needed for a 2-D simulation: 
	
	/*
	cell_defaults.functions.set_orientation = up_orientation; 
	cell_defaults.phenotype.geometry.polarity = 1.0;
	cell_defaults.phenotype.motility.restrict_to_2D = true; 
	*/
	
	// make sure the defaults are self-consistent. 
	/*
	cell_defaults.phenotype.secretion.sync_to_microenvironment( &microenvironment );
	cell_defaults.phenotype.sync_to_functions( cell_defaults.functions ); 

    initialize_cell_definitions_from_pugixml();  //rwh - got commented out

	// set the rate terms in the default phenotype 

	// first find index for a few key variables. 
	static int apoptosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Apoptosis" );
	static int necrosis_model_index = cell_defaults.phenotype.death.find_death_model_index( "Necrosis" );
    
	int oxygen_substrate_index = microenvironment.find_density_index( "oxygen" ); 
 	int glucose_substrate_index = microenvironment.find_density_index( "glucose" );
    int glutamine_substrate_index = microenvironment.find_density_index( "glutamine");
  	int lactate_substrate_index = microenvironment.find_density_index( "lactate" );    


	int Start_index = live.find_phase_index( PhysiCell_constants::live_cells_cycle_model);
	int End_index = live.find_phase_index( PhysiCell_constants::live_cells_cycle_model );
	

	// cell_defaults no necrosis and apoptosis
	cell_defaults.phenotype.death.rates[apoptosis_model_index] = 0.0; 
	cell_defaults.phenotype.death.rates[necrosis_model_index] = 0.0; 
	cell_defaults.phenotype.cycle.data.transition_rate(Start_index,End_index) = 0.0;


	// set oxygen uptake / secretion parameters for the default cell type 
	cell_defaults.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.secretion_rates[oxygen_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.saturation_densities[oxygen_substrate_index] = 0.0; 
    
    cell_defaults.phenotype.secretion.uptake_rates[glucose_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.secretion_rates[glucose_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.saturation_densities[glucose_substrate_index] = 0.0; 
    
    cell_defaults.phenotype.secretion.uptake_rates[glutamine_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.secretion_rates[glutamine_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.saturation_densities[glutamine_substrate_index] = 0.0; 
    
    cell_defaults.phenotype.secretion.uptake_rates[lactate_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.secretion_rates[lactate_substrate_index] = 0.0; 
	cell_defaults.phenotype.secretion.saturation_densities[lactate_substrate_index] = 0.0;     
	
    cell_defaults.custom_data.add_variable( "oxygen_i_conc" , "mmHg", 0.0 );
    cell_defaults.custom_data.add_variable( "glucose_i_conc" , "mMolar", 0.0 );
    cell_defaults.custom_data.add_variable( "glutamine_i_conc", "mMolar", 0.0);
    cell_defaults.custom_data.add_variable( "lactate_i_conc" , "mMolar", 0.0 ); 
    cell_defaults.custom_data.add_variable( "energy", "a.u", 0.0);
    */
	// add custom data here, if any 
	
	
	
	// --------- Defining KRAS Positive Cells -------- //
    /*
	KRAS_positive = cell_defaults; 
	KRAS_positive.type = 3; 
	KRAS_positive.name = "KRAS_positive"; 
	
	// make sure the new cell type has its own reference phenotyhpe
	
	KRAS_positive.parameters.pReference_live_phenotype = &( KRAS_positive.phenotype ); 
	KRAS_positive.phenotype.motility.is_motile = false; 
	// organoid_cell.phenotype.motility.persistence_time = parameters.doubles( "motile_cell_persistence_time" ); // 15.0; // 15 minutes
	// organoid_cell.phenotype.motility.migration_speed = parameters.doubles( "motile_cell_migration_speed" ); // 0.25; // 0.25 micron/minute 
	// organoid_cell.phenotype.motility.migration_bias = 0.0;// completely random 
	
    KRAS_positive.phenotype.molecular.sync_to_microenvironment( &microenvironment );
    KRAS_positive.functions.update_phenotype = tumor_energy_update_function;
    
    
	// Set cell-cell adhesion to 5% of other cells 
	KRAS_positive.phenotype.mechanics.cell_cell_adhesion_strength *= 0.05;  // parameters.doubles( "organoid_cell_relative_adhesion" ); // 0.05; 
	
	// Set apoptosis to zero 
	KRAS_positive.phenotype.death.rates[apoptosis_model_index] = 0.0; //parameters.doubles( "organoid_cell_apoptosis_rate" ); // 0.0; 
	
	// Setting Proliferation
	// 
	KRAS_positive.phenotype.cycle.data.transition_rate(Start_index,End_index) = 0.00021;//parameters.doubles( "organoid_cell_relative_cycle_entry_rate" ); // 0.1; 
	
    // Uptake/Secretion
    KRAS_positive.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 0.5; 
	KRAS_positive.phenotype.secretion.secretion_rates[oxygen_substrate_index] = 0.0; 
	KRAS_positive.phenotype.secretion.saturation_densities[oxygen_substrate_index] = 0.0; 
    
    KRAS_positive.phenotype.secretion.uptake_rates[glucose_substrate_index] = 0.05; 
	KRAS_positive.phenotype.secretion.secretion_rates[glucose_substrate_index] = 0.0; 
	KRAS_positive.phenotype.secretion.saturation_densities[glucose_substrate_index] = 0.0; 
    
    KRAS_positive.phenotype.secretion.uptake_rates[glutamine_substrate_index] = 0.0005; 
	KRAS_positive.phenotype.secretion.secretion_rates[glutamine_substrate_index] = 0.0; 
	KRAS_positive.phenotype.secretion.saturation_densities[glutamine_substrate_index] = 0.0; 
    
    KRAS_positive.phenotype.secretion.uptake_rates[lactate_substrate_index] = 0.0; 
	KRAS_positive.phenotype.secretion.secretion_rates[lactate_substrate_index] = 0.005; 
	KRAS_positive.phenotype.secretion.saturation_densities[lactate_substrate_index] = 10.0;   
    
	// --------- Defining KRAS Negative Cells -------- //
	KRAS_negative = cell_defaults; 
	KRAS_negative.type = 2; 
	KRAS_negative.name = "KRAS_negative"; 
	
	// make sure the new cell type has its own reference phenotyhpe
	
	KRAS_negative.parameters.pReference_live_phenotype = &( KRAS_negative.phenotype ); 
	KRAS_negative.phenotype.motility.is_motile = false; 
	// organoid_cell.phenotype.motility.persistence_time = parameters.doubles( "motile_cell_persistence_time" ); // 15.0; // 15 minutes
	// organoid_cell.phenotype.motility.migration_speed = parameters.doubles( "motile_cell_migration_speed" ); // 0.25; // 0.25 micron/minute 
	// organoid_cell.phenotype.motility.migration_bias = 0.0;// completely random 
	
    KRAS_negative.phenotype.molecular.sync_to_microenvironment( &microenvironment );
    KRAS_negative.functions.update_phenotype = tumor_energy_update_function;
    
    
	// Set cell-cell adhesion to 5% of other cells 
	KRAS_negative.phenotype.mechanics.cell_cell_adhesion_strength *= 0.05;  // parameters.doubles( "organoid_cell_relative_adhesion" ); // 0.05; 
	
	// Set apoptosis to zero 
	KRAS_negative.phenotype.death.rates[apoptosis_model_index] = 0.0; //parameters.doubles( "organoid_cell_apoptosis_rate" ); // 0.0; 
	
	// Setting Proliferation
	// 
	KRAS_negative.phenotype.cycle.data.transition_rate(Start_index,End_index) = 0.00021;//parameters.doubles( "organoid_cell_relative_cycle_entry_rate" ); // 0.1; 
	
    // Uptake/Secretion
    KRAS_negative.phenotype.secretion.uptake_rates[oxygen_substrate_index] = 0.5; 
	KRAS_negative.phenotype.secretion.secretion_rates[oxygen_substrate_index] = 0.0; 
	KRAS_negative.phenotype.secretion.saturation_densities[oxygen_substrate_index] = 0.0; 
    
    KRAS_negative.phenotype.secretion.uptake_rates[glucose_substrate_index] = 0.05; 
	KRAS_negative.phenotype.secretion.secretion_rates[glucose_substrate_index] = 0.0; 
	KRAS_negative.phenotype.secretion.saturation_densities[glucose_substrate_index] = 0.0; 
    
    KRAS_negative.phenotype.secretion.uptake_rates[glutamine_substrate_index] = 0.0005; 
	KRAS_negative.phenotype.secretion.secretion_rates[glutamine_substrate_index] = 0.0; 
	KRAS_negative.phenotype.secretion.saturation_densities[glutamine_substrate_index] = 0.0; 
    
    KRAS_negative.phenotype.secretion.uptake_rates[lactate_substrate_index] = 0.0; 
	KRAS_negative.phenotype.secretion.secretion_rates[lactate_substrate_index] = 0.005; 
	KRAS_negative.phenotype.secretion.saturation_densities[lactate_substrate_index] = 10.0;  	
		
	//------------ Defining Fibroblast ------------//
	
	fibroblast = cell_defaults; 
	fibroblast.type = 2; 
	fibroblast.name = "fibroblast"; 
	
	// make sure the new cell type has its own reference phenotype
	
	fibroblast.parameters.pReference_live_phenotype = &( fibroblast.phenotype ); 
    fibroblast.functions.update_phenotype = tumor_energy_update_function;
	// Set cell-cell adhesion to 5% of other cells 
	//fibro_cell.phenotype.mechanics.cell_cell_adhesion_strength *= parameters.doubles( "fibroblast_relative_adhesion" );
	
	// Set apoptosis to zero 
	fibroblast.phenotype.death.rates[apoptosis_model_index] = 0.0; //parameters.doubles( "fibroblast_apoptosis_rate" ); // 0.0; 
	fibroblast.phenotype.cycle.data.transition_rate(Start_index,End_index) = 0.0;
	fibroblast.phenotype.cycle.data.transition_rate(End_index,Start_index) = 0.0;
	
	fibroblast.phenotype.secretion.uptake_rates[lactate_substrate_index] = 0.1; 
	fibroblast.phenotype.secretion.secretion_rates[lactate_substrate_index] = 0.0; 
	fibroblast.phenotype.secretion.saturation_densities[lactate_substrate_index] = 0.0; 
	
	// ---- END -- Fibroblast Cell Definitions -- END ---- //	
*/
    build_cell_definitions_maps();
    display_cell_definitions(std::cout);
	return; 
}

void setup_microenvironment( void )
{
	// set domain parameters 
	
/*	
	default_microenvironment_options.X_range = {-500, 500}; 
	default_microenvironment_options.Y_range = {-500, 500}; 
	default_microenvironment_options.Z_range = {-500, 500}; 
*/	
	// make sure to override and go back to 2D 
	if( default_microenvironment_options.simulate_2D == true )
	{
		std::cout << "Warning: overriding XML config option and setting to 3D!" << std::endl; 
		default_microenvironment_options.simulate_2D = false; 
	}	
	
	
/*
	// all this is in XML as of August 2019 (1.6.0)
	// no gradients need for this example 

	default_microenvironment_options.calculate_gradients = false; 
	
	// set Dirichlet conditions 

	default_microenvironment_options.outer_Dirichlet_conditions = true;
	
	// if there are more substrates, resize accordingly 
	std::vector<double> bc_vector( 1 , 38.0 ); // 5% o2
	default_microenvironment_options.Dirichlet_condition_vector = bc_vector;
	
	// set initial conditions 
	default_microenvironment_options.initial_condition_vector = { 38.0 }; 
*/
	
	// initialize BioFVM 
	
	initialize_microenvironment(); 	
	
	return; 
}


void setup_1D_microenvironment( void )
{
    Microenvironment coarse_well;
    std::cout << "SPOT 1" <<std::endl;
    
    coarse_well.name = "coarse well";
    coarse_well.spatial_units = "micron";
    coarse_well.mesh.units = "micron";
    coarse_well.time_units = "min";

    coarse_well.set_density( 0 , "oxygen", "mmHg", 1e5 , 0.01 );
    coarse_well.add_density( "glucose", "mmol", 1e5 , 10.0 );
    coarse_well.add_density( "lactate", "mmol", 1e5 , 0.0 );
    coarse_well.resize_space( 100, 1 , 1 );
    
    double dx = 100;
    coarse_well.resize_space_uniform( 0, 5000.0 , -dx/2.0 , dx/2.0 , -dx/2.0 , dx/2.0 , dx );
    std::vector<double> dirichlet_condition = { 1 , 1, 0 };
    
    int my_voxel_index = 0;
    coarse_well.add_dirichlet_node( my_voxel_index , dirichlet_condition );

    dirichlet_condition = { 0,0,0 };
    my_voxel_index = coarse_well.mesh.voxels.size()-1;
    coarse_well.add_dirichlet_node( my_voxel_index , dirichlet_condition );
    coarse_well.diffusion_decay_solver = diffusion_decay_solver__constant_coefficients_LOD_1D;
    
    coarse_well.display_information( std::cout );
    
    
    for( int n = 0 ; n < coarse_well.mesh.voxels.size(); n++ )
    {
        std::cout << coarse_well(n)[0] << " ";
    }
    
    std::cout << std::endl;
    
    double t_temp = 0;
    while( t_temp < 100 )
      {
          coarse_well.simulate_diffusion_decay( 0.01 );
          t_temp += 0.01;
      }

      for( int n = 0 ; n < coarse_well.mesh.voxels.size(); n++ )

      {

            std::cout << coarse_well(n)[0] << " ";

      }
      std::cout << std::endl; 
	return; 
} 

void setup_tissue( void )
{
	// create some cells near the origin
	Cell* pCell;
    int retval;
    static Cell_Definition* pDefault = find_cell_definition("default");
    static Cell_Definition* pFibro = find_cell_definition("fibroblast");
    static Cell_Definition* pKRAS_pos = find_cell_definition("KRAS_positive");
    static Cell_Definition* pKRAS_neg = find_cell_definition("KRAS_negative");

    // 20,000 fibroblast seeding
    int kfib = 0;
    if (parameters.bools("fibroblast_seeding"))
    {
        std::cout << "creating fibroblasts" << std::endl;
        for (int i= -2666; i<2666; i+=16.82+20.8)  //rwh??
        {
            for (int j= -2666; j<2666; j+=16.82+20.8)  //rwh??
            {			
                kfib += 1;
                // pCell = create_cell(fibroblast);
                pCell = create_cell( *pFibro );
                pCell->assign_position(i,-500,j);
            } 	
        }  
    }
    std::cout << "------ created # fibroblasts = " << kfib << std::endl;

    double cell_radius = cell_defaults.phenotype.geometry.radius; 
    double initial_tumor_radius = 46; // parameters.doubles("initial_tumor_radius");

    //rwh
    // double number_of_organoid = 250; //parameters.doubles("number_of_organoid")
    int number_of_organoid = 250; // parameters.doubles("number_of_organoid")
    
                double xmin=1.e6;
                double ymin=1.e6;
                double zmin=1.e6;
                double xmax= -xmin;
                double ymax= -ymin;
                double zmax= -zmin;
	if (parameters.bools("organoid_cell_seeding"))
	{ 
            std::cout << "creating CRCs" << std::endl;
            // rwh: if (parameters.doubles("organoid_cell_seeding_method") == 1)
            if (parameters.ints("organoid_cell_seeding_method") == 1)
            {
                std::cout << "  - seeding method = 1" << std::endl;
                //rwh for (int i = 0; i < number_of_organoid; i++) // seeding number of organoid cells specified in PhysiCell_settings.xml
                int kdx = 0;
                for (int idx = 0; idx < number_of_organoid; idx++) // seeding number of organoid cells specified in PhysiCell_settings.xml
			    {
                    //rwh - why??  and why in this loop?
                    std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius,initial_tumor_radius); 
                    //std::cout << "creating " << positions.size() << " closely-packed organoid cells ... " << std::endl;
                    // create organoid
                        //rwh: create values in range: rand() % (max_number + 1 - minimum_number) + minimum_number; e.g.
                        // [2666, 5333+2666-1] = 2666, 
                        double xrand = (rand() % 5333) - 2666;
                        double yrand = (rand() % 961) - 480;
                        double zrand = (rand() % 5333) - 2666;
                        if (xrand < xmin) xmin = xrand;
                        if (xrand > xmax) xmax = xrand;
                        if (yrand < ymin) ymin = yrand;
                        if (yrand > ymax) ymax = yrand;
                        if (zrand < zmin) zmin = zrand;
                        if (zrand > zmax) zmax = zrand;
                    //std::cout << positions.size() << std::endl;
                    for( int i=0; i < positions.size(); i++ )
                    {
                        positions[i][0] += xrand;//(rand() % 5333) - 2666;
                        positions[i][1] += yrand;//(rand() % 961) - 480;
                        positions[i][2] += zrand;//(rand() % 5333) - 2666;
                        // pCell = create_cell(KRAS_positive);
                        pCell = create_cell( *pKRAS_pos );
                        pCell->assign_position( positions[i] );
                        kdx += 1;
                    }
			    }
                std::cout << " -----  # of KRAS_pos cells = " << kdx << std::endl;
            }
            else if (parameters.doubles("organoid_cell_seeding_method") == 2)
            {
                std::cout << "  - seeding method = 2" << std::endl;
                for (int i = 0; i < number_of_organoid; i++) // seeding number of organoid cells specified in PhysiCell_settings.xml
			    {
                
                    std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius,initial_tumor_radius); 
                    //std::cout << "creating " << positions.size() << " closely-packed organoid cells ... " << std::endl;
                    // create organoid
                        double xrand = (rand() % 5333) - 2666;
                        double yrand = (rand() % 961) - 480;
                        double zrand = (rand() % 5333) - 2666;
                        if (xrand < xmin) xmin = xrand;
                        if (xrand > xmax) xmax = xrand;
                        if (yrand < ymin) ymin = yrand;
                        if (yrand > ymax) ymax = yrand;
                        if (zrand < zmin) zmin = zrand;
                        if (zrand > zmax) zmax = zrand;
                    //std::cout << positions.size() << std::endl;
                    for( int i=0; i < positions.size(); i++ )
                    {
                        positions[i][0] += xrand;//(rand() % 5333) - 2666;
                        positions[i][1] += yrand;//(rand() % 961) - 480;
                        positions[i][2] += zrand;//(rand() % 5333) - 2666;
                        // pCell = create_cell(KRAS_negative);
                        pCell = create_cell( *pKRAS_neg );
                        pCell->assign_position( positions[i] );
                    }
			    }
            }
            else if (parameters.doubles("organoid_cell_seeding_method") == 3)
            {
                std::cout << "  - seeding method = 3" << std::endl;
                for (int i = 0; i < parameters.doubles("percent_KRAS_positive")*number_of_organoid; i++) // seeding number of organoid cells specified in PhysiCell_settings.xml
			    {
                    std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius,initial_tumor_radius); 
                    //std::cout << "creating " << positions.size() << " closely-packed organoid cells ... " << std::endl;
                    // create organoid
                        double xrand = (rand() % 5333) - 2666;
                        double yrand = (rand() % 961) - 480;
                        double zrand = (rand() % 5333) - 2666;
                        if (xrand < xmin) xmin = xrand;
                        if (xrand > xmax) xmax = xrand;
                        if (yrand < ymin) ymin = yrand;
                        if (yrand > ymax) ymax = yrand;
                        if (zrand < zmin) zmin = zrand;
                        if (zrand > zmax) zmax = zrand;
                    //std::cout << positions.size() << std::endl;
                    for( int i=0; i < positions.size(); i++ )
                    {
                        positions[i][0] += xrand;//(rand() % 5333) - 2666;
                        positions[i][1] += yrand;//(rand() % 961) - 480;
                        positions[i][2] += zrand;//(rand() % 5333) - 2666;
                        // pCell = create_cell(KRAS_positive);
                        pCell = create_cell( *pKRAS_pos );
                        pCell->assign_position( positions[i] );
                    }
			    }

                for (int i = 0; i < number_of_organoid - (parameters.doubles("percent_KRAS_positive")*number_of_organoid); i++) // seeding number of organoid cells specified in PhysiCell_settings.xml
			    {
                    std::vector<std::vector<double>> positions = create_cell_sphere_positions(cell_radius,initial_tumor_radius); 
                    //std::cout << "creating " << positions.size() << " closely-packed organoid cells ... " << std::endl;
                    // create organoid
                        double xrand = (rand() % 5333) - 2666;
                        double yrand = (rand() % 961) - 480;
                        double zrand = (rand() % 5333) - 2666;
                    //std::cout << positions.size() << std::endl;
                    for( int i=0; i < positions.size(); i++ )
                    {
                        positions[i][0] += xrand;//(rand() % 5333) - 2666;
                        positions[i][1] += yrand;//(rand() % 961) - 480;
                        positions[i][2] += zrand;//(rand() % 5333) - 2666;
                        // pCell = create_cell(KRAS_negative);
                        pCell = create_cell( *pKRAS_neg );
                        pCell->assign_position( positions[i] );
                    }
			    }
			}
        std::cout << "------- organoid seed cells ranges:\n";
        std::cout << " x: " << xmin << ", " << xmax << std::endl;
        std::cout << " y: " << ymin << ", " << ymax << std::endl;
        std::cout << " z: " << zmin << ", " << zmax << std::endl;
    }

        // pCell->custom_data[i_Lac_i] = pCell->phenotype.intracellular->get_double_parameter_value("Lactate");

        // phenotype.molecular.internalized_total_substrates[i_Lac] = pCell->custom_data[i_Lac_i]*cell_volume;
    //    freeRRCData (result);
}

std::vector<std::string> my_coloring_function( Cell* pCell )
{
	// start with flow cytometry coloring 
	
	std::vector<std::string> output = false_cell_coloring_cytometry(pCell); 
	
	// if the cell is cell and not dead, paint it black 
	
	if( pCell->phenotype.death.dead == false && 
		pCell->type == 1 )
	{
		 output[0] = "red"; 
		 output[2] = "red"; 	
	}
	
	return output; 
}

std::vector<std::vector<double>> create_cell_sphere_positions(double cell_radius, double sphere_radius)
{
	std::vector<std::vector<double>> cells;
	int xc=0,yc=0,zc=0;
	double x_spacing= cell_radius*sqrt(3);
	double y_spacing= cell_radius*2;
	double z_spacing= cell_radius*sqrt(3);
	
	std::vector<double> tempPoint(3,0.0);
	
	for(double z=-sphere_radius;z<sphere_radius;z+=z_spacing, zc++)
	{
		for(double x=-sphere_radius;x<sphere_radius;x+=x_spacing, xc++)
		{
			for(double y=-sphere_radius;y<sphere_radius;y+=y_spacing, yc++)
			{
				tempPoint[0]=x + (zc%2) * 0.5 * cell_radius;
				tempPoint[1]=y + (xc%2) * cell_radius;
				tempPoint[2]=z;
				
				if(sqrt(norm_squared(tempPoint))< sphere_radius)
				{ cells.push_back(tempPoint); }
			}
			
		}
	}
	return cells;
}


void simulate_SBML_for_cell(Cell* pCell, Phenotype& phenotype , double dt)
{   
	// rrc::RRVectorPtr vptr;
	// rrc::RRCDataPtr result;  // start time, end time, and number of points

        // Custom Data indices
    static int i_Oxy_i = pCell->custom_data.find_variable_index( "oxygen_i_conc" );
    static int i_Glu_i = pCell->custom_data.find_variable_index( "glucose_i_conc" );
    static int i_Glt_i = pCell->custom_data.find_variable_index( "glutamine_i_conc" );
    static int i_Lac_i = pCell->custom_data.find_variable_index( "lactate_i_conc" );
    static int energy_vi = pCell->custom_data.find_variable_index( "energy" );

	if( pCell->phenotype.death.dead == false && pCell->type == 1 )
    {
        // SBML indices
        // static int SBML_idx_glucose = 0;
        // static int SBML_idx_oxygen = 1;
        // static int SBML_idx_energy = 2;
        // static int SBML_idx_lactate = 3;
        // static int SBML_idx_glutamine = 4;

        // rrc::RRVectorPtr vptr;
        // rrc::RRCDataPtr result;

        // BioFVM indices
        static int i_Oxy = microenvironment.find_density_index( "oxygen" ); 
        static int i_Glu = microenvironment.find_density_index( "glucose" );
        static int i_Glt = microenvironment.find_density_index( "glutamine" );
        static int i_Lac = microenvironment.find_density_index( "lactate" );
        
        // Internal Amounts
        double internal_oxygen = phenotype.molecular.internalized_total_substrates[i_Oxy];
        double internal_glucose = phenotype.molecular.internalized_total_substrates[i_Glu];
        double internal_glutamine = phenotype.molecular.internalized_total_substrates[i_Glt];
        double internal_lactate = phenotype.molecular.internalized_total_substrates[i_Lac];
        //std::cout << internal_oxygen << "," << phenotype.volume.total << std::endl;
        
        // Custom Data indices
        // static int i_Oxy_i = pCell->custom_data.find_variable_index( "oxygen_i_conc" );
        // static int i_Glu_i = pCell->custom_data.find_variable_index( "glucose_i_conc" );
        // static int i_Glt_i = pCell->custom_data.find_variable_index( "glutamine_i_conc" );
        // static int i_Lac_i = pCell->custom_data.find_variable_index( "lactate_i_conc" );
        // static int energy_vi = pCell->custom_data.find_variable_index( "energy" );

        double cell_volume = phenotype.volume.total;

        // Calculating internal concentrations & Updating cell data
        pCell->custom_data[i_Oxy_i] = internal_oxygen / cell_volume;
        pCell->custom_data[i_Glu_i] = internal_glucose / cell_volume;
        pCell->custom_data[i_Lac_i] = internal_lactate / cell_volume;
        pCell->custom_data[i_Glt_i] = internal_glutamine / cell_volume;
/*         if (pCell->custom_data[i_Oxy_i] < 0)
        { std::cout <<  pCell->custom_data[i_Oxy_i]  << std::endl; }
        if (pCell->custom_data[i_Glu_i] < 0)
        { std::cout <<  pCell->custom_data[i_Glu_i]  << std::endl; }
        if (pCell->custom_data[i_Lac_i] < 0)
        { std::cout <<  pCell->custom_data[i_Lac_i]  << std::endl; }
        if (pCell->custom_data[i_Glt_i] < 0)
        { std::cout <<  pCell->custom_data[i_Glt_i]  << std::endl; }
         */
        // ! NO Energy Update is required !
        //std::cout <<  "Internal Oxygen Amount: " << internal_oxygen  << std::endl;
        //std::cout <<  "Internal Oxygen Concentration: " << pCell->custom_data[i_Oxy_i]  << std::endl;

        //std::cout <<  "Internal Glucose Amount: " << internal_glucose  << std::endl;
        //std::cout <<  "Internal Glucose Concentration: " << pCell->custom_data[i_Glu_i]  << std::endl;

        // Geting molecular model structure
        // vptr = rrc::getFloatingSpeciesConcentrations(pCell->phenotype.molecular.model_rr);
        
        // Setting New Values to SBML
        // vptr->Data[SBML_idx_oxygen] = pCell->custom_data[i_Oxy_i];
        // vptr->Data[SBML_idx_glucose] = pCell->custom_data[i_Glu_i];
        // vptr->Data[SBML_idx_lactate] = pCell->custom_data[i_Lac_i];
        // vptr->Data[SBML_idx_glutamine] = pCell->custom_data[i_Glt_i];
        // vptr->Data[SBML_idx_energy] = pCell->custom_data[energy_vi];

        int retval;
        retval = pCell->phenotype.intracellular->set_parameter_value("Oxygen", pCell->custom_data[i_Oxy_i]);
        retval = pCell->phenotype.intracellular->set_parameter_value("Glucose", pCell->custom_data[i_Glu_i]);
        retval = pCell->phenotype.intracellular->set_parameter_value("Lactate", pCell->custom_data[i_Lac_i]);
        retval = pCell->phenotype.intracellular->set_parameter_value("Glutamine", pCell->custom_data[i_Glt_i]);
        retval = pCell->phenotype.intracellular->set_parameter_value("Energy", pCell->custom_data[energy_vi]);
        
        // rrc::setFloatingSpeciesConcentrations(pCell->phenotype.molecular.model_rr, vptr);
        
        //std::cout << "Before Simulation Glucose: " << vptr->Data[SBML_idx_glucose] << std::endl;
        // SBML Simulation
        // result = rrc::simulateEx (pCell->phenotype.molecular.model_rr, 0, 0.01, 2);  // start time, end time, and number of points
        retval = pCell->phenotype.intracellular->update();
        
        //std::cout << result->ColumnHeaders[0] << result->Data[6] << std::endl;
        //std::cout << "After Simulation Energy: " << result->Data[8] << std::endl;
        
    /*     std::cout << "--- after simulation:" << std::endl;
        for (int idx=0; idx<vptr->Count; idx++)
        {
            std::cout << idx << ", " << vptr->Data[idx] << std::endl;
        } */
        /////////---------------Simulation Demo-----------------------///////
        //int idx = (result->RSize - 1) * result->CSize + 1;
        //std::cout << "Cell ID 0) Saving last energy value (cell custom var) = " << result->Data[idx] << std::endl;
    /*     for (int idx=0; idx<20; idx++)
        {
            std::cout << idx << ", " << result->Data[8] << std::endl;
        } */

        // Result Indicing!!!!!
        // pCell->custom_data[i_Glu_i] = result->Data[7];
        // pCell->custom_data[i_Oxy_i] = result->Data[8];
        // pCell->custom_data[energy_vi] = result->Data[9];
        // //std::cout << "Energy: " << pCell->custom_data[energy_vi] << std::endl;
        // pCell->custom_data[i_Lac_i] = result->Data[10];
        // pCell->custom_data[i_Glt_i] = result->Data[11];

        pCell->custom_data[i_Glu_i] = pCell->phenotype.intracellular->get_double_parameter_value("Glucose");
        pCell->custom_data[i_Oxy_i] = pCell->phenotype.intracellular->get_double_parameter_value("Oxygen");
        pCell->custom_data[energy_vi] = pCell->phenotype.intracellular->get_double_parameter_value("Energy");
        pCell->custom_data[i_Lac_i] = pCell->phenotype.intracellular->get_double_parameter_value("Lactate");
        pCell->custom_data[i_Glt_i] = pCell->phenotype.intracellular->get_double_parameter_value("Glutamine");
        
        
        phenotype.molecular.internalized_total_substrates[i_Glu] = pCell->custom_data[i_Glu_i]*cell_volume;
        phenotype.molecular.internalized_total_substrates[i_Oxy] = pCell->custom_data[i_Oxy_i]*cell_volume;
        phenotype.molecular.internalized_total_substrates[i_Lac] = pCell->custom_data[i_Lac_i]*cell_volume;
        phenotype.molecular.internalized_total_substrates[i_Glt] = pCell->custom_data[i_Glt_i]*cell_volume;
        // freeRRCData (result);
    }

    if( pCell->phenotype.death.dead == false && pCell->type == 2 )
    {
        // SBML indices
        static int SBML_idx_lactate = 0;
        int retval;

        // rrc::RRVectorPtr vptr;
        // rrc::RRCDataPtr result;

        // BioFVM indices
        static int i_Lac = microenvironment.find_density_index( "lactate" );
        
        // Internal Amounts
        double internal_lactate = phenotype.molecular.internalized_total_substrates[i_Lac];
        //std::cout << internal_oxygen << "," << phenotype.volume.total << std::endl;
        
        // Custom Data indices
        // double i_Lac_i = pCell->custom_data.find_variable_index( "lactate_i_conc" );
        double cell_volume = phenotype.volume.total;

        // Calculating internal concentrations & Updating cell data
        pCell->custom_data[i_Lac_i] = internal_lactate / cell_volume;
        

        // Geting molecular model structure
        // vptr = rrc::getFloatingSpeciesConcentrations(pCell->phenotype.molecular.model_rr);
        
        // Setting New Values to SBML
        // vptr->Data[SBML_idx_lactate] = pCell->custom_data[i_Lac_i];
        retval = pCell->phenotype.intracellular->set_parameter_value("Lactate", pCell->custom_data[i_Lac_i]);
        
        // rrc::setFloatingSpeciesConcentrations(pCell->phenotype.molecular.model_rr, vptr);
        
        //std::cout << "Before Simulation Glucose: " << vptr->Data[SBML_idx_glucose] << std::endl;
        // SBML Simulation
        // result = rrc::simulateEx (pCell->phenotype.molecular.model_rr, 0, 0.01, 2);  // start time, end time, and number of points
        retval = pCell->phenotype.intracellular->update();
 
        //std::cout << result->ColumnHeaders[1] << result->Data[3] << std::endl;
        //std::cout << "After Simulation Energy: " << result->Data[8] << std::endl;
        // Result Indicing!!!!!
        //std::cout << "Energy: " << pCell->custom_data[energy_vi] << std::endl;
        // pCell->custom_data[i_Lac_i] = result->Data[3];
        pCell->custom_data[i_Lac_i] = pCell->phenotype.intracellular->get_double_parameter_value("Lactate");

        phenotype.molecular.internalized_total_substrates[i_Lac] = pCell->custom_data[i_Lac_i]*cell_volume;
    //    freeRRCData (result);
    }
}

void simulate_SBML_for_all_cells(void) 
{
    #pragma omp parallel for
    for( int i=0; i < (*all_cells).size(); i++ )
    {
        //std::cout << "simulating_SBML_for_all_cells test" << std::endl;
        simulate_SBML_for_cell((*all_cells)[i], (*all_cells)[i]->phenotype , 0.01);
    }
} 