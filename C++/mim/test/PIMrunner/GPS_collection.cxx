// GPS_collection.cxx
// code for dealing with ionosphere predictions from GPS data
//_HIST  DATE NAME PLACE INFO
//	2007 Jan 02  James M Anderson  --JIVE  start
//	2007 Apr 19  JMA  --update for satellite track changes




// INCLUDES
#include "JMA_code.h"
#include "JMA_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "GPS_collection.h"
#include "station_maker.h"
#include "physical_constants.h"
#include "linfit.h"
#include "vex_time.h"
#include <vector>
#include <assert.h>
#include "ionosphere_gen.h"


namespace {
    const char GPS_GLOBAL_STATION_LIST[][5] = {"abmf", "abpo", "ac23", "ac24", "acrg", "acso", "adis", "aggo", "aira", "ajac", "albh", "algo", "alic", "alrt", "amc4", "ank2", "anmg", "antc", "antf", "areg", "areq", "arht", "artu", "aruc", "ascg", "aspa", "atru", "auck", "badg", "baie", "bake", "bako", "bamf", "barh", "baut", "bele", "bhr3", "bhr4", "bik0", "bill", "bjco", "bjfs", "bjnm", "blyt", "bnoa", "boav", "bogi", "bogt", "bor1", "braz", "brew", "brft", "brmg", "brst", "brun", "brux", "bshm", "btng", "bucu", "bzr2", "cags", "cas1", "ccj2", "cebr", "cedu", "cggn", "chan", "chil", "chof", "chpg", "chpi", "chti", "chum", "chur", "chwk", "cibg", "cit1", "ckis", "cksv", "cmp9", "cmum", "cnmr", "coco", "cord", "coso", "cote", "coyq", "cpnm", "cpvg", "crfp", "cro1", "cuib", "cusv", "cut0", "cuut", "cyne", "cztg", "dae2", "daej", "dakr", "darw", "dav1", "dear", "dgar", "dhlg", "djig", "dlf1", "dltv", "drag", "drao", "dubo", "dumg", "dund", "dyng", "ebre", "eil3", "eil4", "enao", "eprt", "escu", "faa1", "fair", "falk", "ffmj", "flin", "flrs", "frdn", "ftna", "func", "gamb", "gamg", "ganp", "gcgo", "geno", "glps", "glsv", "gode", "godn", "gods", "godz", "gol2", "gold", "gop6", "gop7", "gope", "grac", "gras", "graz", "guam", "guat", "guug", "hal1", "hamd", "harb", "hers", "hert", "hksl", "hkws", "hlfx", "hnlc", "hnpt", "hnus", "hob2", "hofn", "holb", "holm", "holp", "hrag", "hrao", "hueg", "hyde", "ieng", "iisc", "iitk", "ineg", "invk", "iqal", "iqqe", "irkj", "irkm", "isba", "ishi", "ispa", "ista", "izmi", "jctw", "jdpr", "jfng", "jnav", "jog2", "joz2", "joze", "jplm", "jpre", "karr", "kat1", "kerg", "khar", "kir0", "kir8", "kiri", "kiru", "kit3", "kitg", "kmnm", "kokb", "kokv", "kos1", "kost", "kouc", "koug", "kour", "krgg", "krs1", "ksu1", "kuj2", "kzn2", "lae1", "lama", "laut", "lbch", "lck3", "lck4", "leij", "lhaz", "licc", "llag", "lmmf", "lpal", "lpgs", "lroc", "m0se", "mac1", "mad2", "madr", "mag0", "maju", "mal2", "mana", "mar6", "mar7", "mars", "mas1", "mat1", "mate", "matg", "maui", "maw1", "mayg", "mbar", "mchl", "mcil", "mcm4", "mdo1", "mdvj", "medi", "meli", "mers", "met3", "metg", "mets", "mfkg", "mgue", "mikl", "mizu", "mkea", "mobj", "mobk", "mobn", "mobs", "moiu", "monp", "morp", "mqzg", "mrc1", "mrl1", "mrl2", "mro1", "mssa", "mtka", "mtv1", "mtv2", "nabg", "nain", "nano", "naur", "naus", "ncku", "nico", "nist", "nium", "nklg", "nlib", "nnor", "not1", "novm", "nrc1", "nril", "nrmd", "ntus", "nvsk", "nya1", "nya2", "nyal", "oak1", "oak2", "obe4", "ohi2", "ohi3", "ons1", "onsa", "op71", "opmt", "orid", "osn3", "osn4", "ous2", "owmg", "p043", "p051", "p053", "p389", "p779", "p802", "pado", "palm", "parc", "park", "pbr4", "pdel", "penc", "pert", "pets", "pgen", "picl", "pie1", "pimo", "pin1", "pngm", "poal", "pohn", "pol2", "polv", "pots", "pove", "pppc", "prds", "pre3", "pre4", "ptag", "ptbb", "ptgg", "ptvl", "qaq1", "qiki", "qui3", "qui4", "quin", "rabt", "raeg", "ramo", "rbay", "rdsd", "redu", "reso", "reun", "reyk", "rgdg", "riga", "rio2", "riop", "roag", "rock", "roth", "salu", "samo", "sant", "sask", "savo", "sbok", "sch2", "scip", "scor", "scrz", "sctb", "scub", "sejn", "seme", "sey2", "seyg", "sfdm", "sfer", "sgoc", "sgpo", "shao", "she2", "shlg", "sin1", "smst", "sni1", "sod3", "sofi", "solo", "spk1", "spt0", "sptu", "ssia", "stfu", "sthl", "stj3", "stjo", "stk2", "stpm", "str1", "str2", "sulp", "suth", "sutm", "suwn", "svtl", "sydn", "syog", "tabl", "tana", "tash", "tcms", "tdou", "tehn", "thtg", "thti", "thu2", "tid1", "tidb", "tit2", "tixi", "tlse", "tlsg", "tnml", "tong", "topl", "torp", "tow2", "trak", "tro1", "tsk2", "tskb", "tubi", "tuva", "twtf", "ucal", "uclp", "uclu", "ufpr", "ulab", "uldi", "unb3", "unbd", "unbj", "unbn", "unsa", "ural", "urum", "uscl", "usn7", "usn8", "usn9", "usp1", "usud", "utqi", "uzhl", "vacs", "vald", "vill", "vis0", "vndp", "voim", "wab2", "wark", "warn", "wdc5", "wdc6", "wes2", "wgtn", "whc1", "whit", "widc", "will", "wind", "wlsn", "wroc", "wsrt", "wtz3", "wtza", "wtzr", "wtzs", "wtzz", "wuh2", "wuhn", "wuth", "xmis", "yakt", "yar2", "yar3", "yarr", "yebe", "yel2", "yell", "yibl", "ykro", "yons", "yssk", "zamb", "zeck", "zim2", "zim3", "zimm"};
    const Uint32 GPS_GLOBAL_STATION_LIST_SIZE = sizeof(GPS_GLOBAL_STATION_LIST)/5;
        }


// set up a namespace area for stuff.
namespace MIM_PIM {




// GLOBALS








    


// FUNCTIONS

GPS_collection::
GPS_collection(Uint32 Num_Receivers_,
               GPS_Fit_Type_Enum Default_Fit_Type_,
               Real32 Max_Sat_Sky_Angle_,
               Real32 Min_Sat_Elev_,
               Real32 Max_Rec_Dist_From_Tele_,
               Real32 Max_Iono_Pierce_Dist_,
               Real32 Default_Iono_Height_,
               Real32 Averaging_Time_Half_Width_,
               Uint32 Num_Ionosphere_Parameters_,
               Uint32 Num_Ionosphere_Heights_,
               Uint32 Num_Time_Terms_,
               Ionosphere_Theoretical_Model_Enum Theo_Model_Type_,
               GPS_Bias_Fit_Type_Enum Bias_Fit_Type_
               ) : Num_Receivers(Num_Receivers_),
                   Next_Receiver_Fill(0),
                   time_arr(NULL),
                   sat_pos(NULL),
                   rec_list(NULL),
                   rec_templist(NULL),
                   selection_criteria(Default_Fit_Type_,
                                      Max_Sat_Sky_Angle_,
                                      Min_Sat_Elev_,
                                      Max_Rec_Dist_From_Tele_,
                                      Max_Iono_Pierce_Dist_,
                                      Default_Iono_Height_,
                                      Averaging_Time_Half_Width_,
                                      Num_Ionosphere_Parameters_,
                                      Num_Ionosphere_Heights_,
                                      Num_Time_Terms_,
                                      Theo_Model_Type_,
                                      Bias_Fit_Type_
                                      ),
                   Max_Number_Observations(0),
                   Max_Number_Workspace(0),
                   observations_2D(NULL),
                   observations_3D(NULL),
                   use_flag(NULL),
                   workspace(NULL),
                   satellite_workspace(NULL),
                   satellite_bias_workspace(NULL),
                   Total_Bias_Correction_Done_Flag(0),
                   satellite_track_main_ID(NULL),
                   satellite_track_track_pos(NULL),
                   satellite_track_reciever_pos(NULL),
                   satellite_track_satellite_pos(NULL),
                   Satellite_Track_Max_Tracks(0)
{
    rec_list = new GPS_receiver*[Num_Receivers];
    rec_templist = new GPS_receiver*[Num_Receivers];
    for(Uint32 i=0; i < Num_Receivers; i++)
        rec_templist[i] = rec_list[i] = NULL;
    satellite_workspace = new Uint32[GPS_MAX_POSSIBLE_SATELLITES];
    satellite_bias_workspace = new Real64[GPS_MAX_POSSIBLE_SATELLITES];
    Num_Valid_Receivers = 0;
    return;
}

GPS_collection::~GPS_collection(void)
{
    delete_observation_memory();
    reallocate_satellite_track_workspace(0);
    if((rec_list)) {
        for(Uint32 i=0; i < Num_Receivers; i++) {
            if((rec_list[i])) delete rec_list[i];
        }
        delete[] rec_list;
    }
    if((rec_templist)) delete[] rec_templist;
    if((sat_pos)) delete sat_pos;
    if((time_arr)) delete time_arr;
    if((satellite_workspace)) delete satellite_workspace;
    if((satellite_bias_workspace)) delete satellite_bias_workspace;
    return;
}



Sint32 GPS_collection::fill_receiver_data(const char name[],
                                          const Space_Vector& position,
                                          const Uint32 N_TIMES,
                                          const Uint32 N_SAT,
                                          const Sint16* const sat_data,
                                          const Sint16* const track_data,
                                          const Real64* const stec_data,
                                          const Real64* const sigma_data,
                                          const Sint32* const bias_valid)
{
    if((rec_list[Next_Receiver_Fill]))
        delete rec_list[Next_Receiver_Fill];
    rec_list[Next_Receiver_Fill] = NULL;
    rec_list[Next_Receiver_Fill] = new GPS_receiver(name,
                                                    position,
                                                    N_TIMES, N_SAT,
                                                    sat_data,
                                                    track_data,
                                                    stec_data,
                                                    sigma_data,
                                                    bias_valid);
    if(rec_list[Next_Receiver_Fill]->station_obs.valid_data_exists()) {
        printf("GPS_collection loaded station '%4s' into slot %4u/%u\n",
               rec_list[Next_Receiver_Fill]->station_name(),
               Next_Receiver_Fill,Num_Receivers);
        Num_Valid_Receivers++;
        Next_Receiver_Fill++;
        if(Next_Receiver_Fill == Num_Receivers) Next_Receiver_Fill = 0;
        Total_Bias_Correction_Done_Flag = 0;
    }
    else {
        fprintf(stderr, "GPS_collection failed to load station '%4s' into because there was no valid data\n",name);
        delete rec_list[Next_Receiver_Fill];
        rec_list[Next_Receiver_Fill] = NULL;
    }
    return test_object_sizes();
}
    

void GPS_collection::reset_selection_criteria(
    GPS_Fit_Type_Enum Default_Fit_Type_,
    Real32 Max_Sat_Sky_Angle_,
    Real32 Min_Sat_Elev_,
    Real32 Max_Rec_Dist_From_Tele_,
    Real32 Max_Iono_Pierce_Dist_,
    Real32 Default_Iono_Height_,
    Real32 Averaging_Time_Half_Width_,
    Uint32 Num_Ionosphere_Parameters_,
    Uint32 Num_Ionosphere_Heights_,
    Uint32 Num_Time_Terms_,
    Ionosphere_Theoretical_Model_Enum Theo_Model_Type_,
    GPS_Bias_Fit_Type_Enum Bias_Fit_Type_
    
    ) throw()
{
    if(Theo_Model_Type_ != selection_criteria.Theo_Model_Type) {
        for(Uint32 i=0; i < Num_Receivers; i++) {
            if((rec_list[i])) {
                rec_list[i]->station_obs.blank_model_data();
            }
        }
    }
    selection_criteria = GPS_criteria(Default_Fit_Type_,
                                      Max_Sat_Sky_Angle_,
                                      Min_Sat_Elev_,
                                      Max_Rec_Dist_From_Tele_,
                                      Max_Iono_Pierce_Dist_,
                                      Default_Iono_Height_,
                                      Averaging_Time_Half_Width_,
                                      Num_Ionosphere_Parameters_,
                                      Num_Ionosphere_Heights_,
                                      Num_Time_Terms_,
                                      Theo_Model_Type_,
                                      Bias_Fit_Type_
                                      );
    return;
}




Sint32 GPS_collection::test_object_sizes(void) const throw()
{
    // make sure that each object has the same number of times
    Uint32 n = 0;
    if((time_arr)) n = time_arr->NUM_TIMES;
    else if((sat_pos)) n = sat_pos->NUM_TIMES;
    else if((rec_list[0])) n = rec_list[0]->station_obs.NUM_TIMES;
    else return 0;
    if((time_arr)) {
        if(n != time_arr->NUM_TIMES) return -1;
    }
    if((sat_pos)) {
        if(n != sat_pos->NUM_TIMES) return -2;
    }
    if((rec_list[0])) {
        if(n != rec_list[0]->station_obs.NUM_TIMES) return -3;
        if(Next_Receiver_Fill > 0) {
            if(n != rec_list[Next_Receiver_Fill-1]->station_obs.NUM_TIMES)
                return -4;
        }
        else if((rec_list[Num_Receivers-1])) {
            if(n != rec_list[Num_Receivers-1]->station_obs.NUM_TIMES)
                return -5;
        }
    }
    return 0;
}


void GPS_collection::count_total_tracks(void)
{
    Uint32 total_tracks = 0;
    for(Uint32 i=0; i < Num_Receivers; i++) {
        if((rec_list[i])) {
            Uint32 rec_track_count = rec_list[i]->station_obs.total_tracks();
            total_tracks += rec_track_count;
        }
    }
    fprintf(stderr, "Found %u total satellite/station tracks\n", total_tracks);
    reallocate_satellite_track_workspace(total_tracks);
    Uint32 last = 0;
    Uint32 next = 0;
    for(Uint32 r=0; r < Num_Receivers; r++) {
        Uint32* pos = satellite_track_main_ID + r*GPS_MAX_POSSIBLE_SATELLITES;
        if((rec_list[r])) {
            rec_list[r]->station_obs.total_tracks(pos);
            for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++) {
                last += next;
                next = pos[s];
                pos[s] = last;
            }
        }
        else {
            last += next;
            next = 0;
            for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++) {
                pos[s] = last;
            }
        }
    }
    return;
}


void GPS_collection::delete_observation_memory(const bool all)
{
    Max_Number_Observations = 0;
    if(all) {
        Max_Number_Workspace = 0;
        if((workspace)) delete[] workspace;
        workspace = NULL;
    }
    if((use_flag)) delete[] use_flag;
    use_flag = NULL;
    if((observations_2D)) delete[] observations_2D;
    observations_2D = NULL;
    if((observations_3D)) delete[] observations_3D;
    observations_3D = NULL;
    return;
}

void GPS_collection::reallocate_2D_observations(const Uint32 NUM_OBS,
                                                const Uint32 NUM_WORK)
{
    if((NUM_OBS > Max_Number_Observations) || (observations_2D == NULL)) {
        delete_observation_memory(false);
        use_flag = new bool[NUM_OBS];
        observations_2D = new Observation_Ionosphere[NUM_OBS];
        Max_Number_Observations = NUM_OBS;
    }
    if(NUM_WORK > Max_Number_Workspace) {
        Max_Number_Workspace = 0;
        if((workspace)) delete[] workspace;
        workspace = new Real64[NUM_WORK];
        Max_Number_Workspace = NUM_WORK;
    }
    return;
}

void GPS_collection::reallocate_3D_observations(const Uint32 NUM_OBS,
                                                const Uint32 NUM_WORK)
{
    if((NUM_OBS > Max_Number_Observations) || (observations_3D == NULL)) {
        delete_observation_memory(false);
        use_flag = new bool[NUM_OBS];
        observations_3D = new Observation_3D_Ionosphere[NUM_OBS];
        Max_Number_Observations = NUM_OBS;
    }
    if(NUM_WORK > Max_Number_Workspace) {
        Max_Number_Workspace = 0;
        if((workspace)) delete[] workspace;
        workspace = new Real64[NUM_WORK];
        Max_Number_Workspace = NUM_WORK;
    }
    return;
}





void GPS_collection::reallocate_satellite_track_workspace(
    const Uint32 NUM_TRACKS)
{
    if((NUM_TRACKS == 0) && (satellite_track_main_ID != NULL)) {
        // deallocate everything
        delete[] satellite_track_main_ID;
        satellite_track_main_ID = NULL;
        satellite_track_reciever_pos = NULL;
        satellite_track_satellite_pos = NULL;
        satellite_track_track_pos = NULL;
        Satellite_Track_Max_Tracks = 0;
    }
    else if(NUM_TRACKS > Satellite_Track_Max_Tracks) {
        reallocate_satellite_track_workspace(0); // delete old space first
        // Now allocate memory.  Note that this can be done all at once
        // and stuffed into a sequence of positions.
        satellite_track_main_ID = new Uint32[NUM_TRACKS*3
                                             + Num_Receivers *
                                             GPS_MAX_POSSIBLE_SATELLITES];
        satellite_track_track_pos = satellite_track_main_ID
            + Num_Receivers * GPS_MAX_POSSIBLE_SATELLITES;
        satellite_track_reciever_pos = satellite_track_track_pos+NUM_TRACKS;
        satellite_track_satellite_pos =
            (Sint16*)(satellite_track_reciever_pos+NUM_TRACKS);
        Satellite_Track_Max_Tracks = NUM_TRACKS;
    }
    return;
}





Real32 GPS_collection::get_receiver_obs_model_STEC(
    Ionosphere_Base* const model_ionosphere,
    GPS_receiver* const Receiver,
    const Real64 MJD,
    const Space_Unit_Vector& satellite,
    const Uint32 time_slot,
    const Uint32 satellite_slot
    )
{
    // Is there any data already available?
    Real64 model_STEC = GPS_BAD_DATA_VALUE;
    Receiver->station_obs.get_model_data(time_slot, satellite_slot, &model_STEC);
    if(model_STEC != GPS_BAD_DATA_VALUE) return model_STEC;
    // If here, then no model data available.  Get the model value, store in the
    // observation area, and return
    if(!model_ionosphere) return GPS_BAD_DATA_VALUE;
    Real64 iono_model_STEC = model_ionosphere->Integrated_Electron_Density(
        Receiver->station_position,
        MJD,
        satellite,
        Receiver->station_position.Elevation());
    if(iono_model_STEC > -1.0E300) model_STEC = iono_model_STEC;
    Receiver->station_obs.set_model_data(time_slot, satellite_slot, model_STEC);
    return model_STEC;
}





    
GPS_receiver* GPS_collection::get_nearest_receiver(
    const LatLon_Cart& telescope) const
{
    // Find the GPS receiver closest to the given telescope position.
    const Space_Vector T = telescope;

    // If there are no GPS receivers in the dataset, this can return NULL
    GPS_receiver* closest_receiver = NULL;
    Real64 closest_distance = 1E300; // m, some really huge number

    for(Uint32 i=0; i < Num_Receivers; i++) {
        if((rec_list[i])) {
            Real64 dist = (rec_list[i]->station_position - T).Magnitude();
            if(dist < closest_distance) {
                closest_distance = dist;
                closest_receiver = rec_list[i];
            }
        }
    }
    return closest_receiver;
}




//_TITLE  get_nearby_receivers --get receivers close to some station
void GPS_collection::get_nearby_receivers(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const LatLon_Cart& telescope,
                               // I  the telescope position to search around
    GPS_receiver**& close_rec_list,
                               // O  A pointer to a list of GPS receivers.
                               //    The caller must not delete[] this array.
    Uint32& num_close_receivers
                               // O  The total number of close receivers
                               //    which have been placed into
                               //    close_rec_list.
    ) const

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function searches through the existing list of receivers, and
// 	finds the receivers which are close to the provided telescope position.

//	Here, close is defined as being within Max_Rec_Dist_From_Tele.  Note
//	that this distance is a linear distance calculation, and not a
//	curvilinear distance on the surface of the Earth.

//_FILE  files used and logical units used

//_LIMS  design limitations
//	The distance calculation is a linear distance.

//	The orginal value of close_rec_list is ignored.  The caller is
//	responsible for freeing any memory from this before calling.  

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 02  James M Anderson  --JIVE  start
//      2007 Jan 12  JMA  --use internal array for close_rec_list
//      2007 Feb 23  JMA  --use new GPS_criteria object

//_END

{
    // Find the GPS receiver closest to the given telescope position.
    const Space_Vector T = telescope;

    // allocate the memory to hold the new list
    close_rec_list = rec_templist;
    for(Uint32 i=0; i < Num_Valid_Receivers; i++) close_rec_list[i] = NULL;
    Uint32 pos = 0;

    for(Uint32 i=0; i < Num_Valid_Receivers; i++) {
        if((rec_list[i])) {
            Real64 dist = (rec_list[i]->station_position - T).Magnitude();
//          fprintf(stderr, "For receiver %u got dist %E\n", i, dist);
            if(dist < selection_criteria.Max_Rec_Dist_From_Tele) {
                close_rec_list[pos++] = rec_list[i];
            }
        }
    }
    num_close_receivers = pos;
    return;
}






//_TITLE  get_global_receivers --get all global receivers in the collection
void GPS_collection::get_global_receivers(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    GPS_receiver**& global_rec_list,
                               // O  A pointer to a list of GPS receivers.
                               //    The caller must not delete[] this array.
    Uint32& num_global_receivers
                               // O  The total number of global receivers
                               //    which have been placed into
                               //    global_rec_list.
    )

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function returns all of the global reference stations which are
//	in the collection.

//_FILE  files used and logical units used

//_LIMS  design limitations
//	The orginal value of close_rec_list is ignored.  The caller is
//	responsible for freeing any memory from this before calling.  

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 mar 23  James M Anderson  --JIVE  start

//_END

{
    // allocate the memory to hold the new list
    global_rec_list = rec_templist;
    for(Uint32 i=0; i < Num_Receivers; i++) global_rec_list[i] = NULL;
    Uint32 pos = 0;

    for(Uint32 i=0; i < Num_Valid_Receivers; i++) {
        if((rec_list[i])) {
            // Is the station name in the global list?
            bool found = false;
            for(Uint32 j=0; j < GPS_GLOBAL_STATION_LIST_SIZE; j++) {
                if(strcmp(GPS_GLOBAL_STATION_LIST[j],
                          rec_list[i]->station_name()) == 0) {
                    found = true;
                    break;
                }
            }
            if(found) {
                global_rec_list[pos++] = rec_list[i];
            }
        }
    }
    num_global_receivers = pos;
    return;
}










//_TITLE  test_satellite_criteria_fail --is the satellite outside limits
int GPS_collection::test_satellite_criteria_fail(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Space_Vector& target_pierce,
                               // I  The pierce point of the target observation
                               //    from the telescope, in Earth-fixed
                               //    coordinates
    const Station_LatLon& receiver,
                               // I  The GPS receiver position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& satellite,
                               // I  The satellite direction from the
                               //    viewpoint of the receiver, in Earth-fixed
                               //    coordinates.
    const Real64 Separation_Angle,
                               // I  The angle between the target vector and
                               //    the satellite vector in radians.
    const Real64 Sat_Elevation_Angle
                               // I  The elevation angle of the satellite as
                               //    seen by the receiver, in radians.
    ) const
// int GPS_collection::test_satellite_criteria_fail
//                                O  Flag to indicate whether the satellite
//                                   is within the boundary conditions.
//                                    0 no failure, satellite is good
//                                   -1 angular distance failure
//                                   -2 elevation angle failure
//                                   -3 intercept point distance failure

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Feb 23  James M Anderson  --revise to use new GPS_criteria object

//_END

{
    // First, test that the target source and the satellite are roughly
    // in the same direction.
    {
        if(Separation_Angle > selection_criteria.Max_Sat_Sky_Angle) return -1;
    }
    // Now test the satellite elevation angle
    {
        if(Sat_Elevation_Angle < selection_criteria.Min_Sat_Elev) return -2;
    }
    // Now test for the ionosphre intercept point distance
    {
        Space_Vector pierce =
            receiver.get_pierce_location(satellite,
                                         selection_criteria.Default_Iono_Height,
                                         radius_Earth);
        Real64 dist = (pierce - target_pierce).Magnitude();
        if(dist > selection_criteria.Max_Iono_Pierce_Dist) return -3;
    }
    // If we have gotten here, then we have passed all tests
    return 0;        
}
    










//_TITLE  get_single_receiver_nearest_STEC --
GPS_collection_return GPS_collection::get_single_receiver_nearest_STEC(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs       // I  The time of the observation, as an MJD.
    )
// GPS_collection_return GPS_collection::get_single_receiver_nearest_STEC
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.
//                                   The SRM and sigma_SRM are always returned as
//                                   GPS_BAD_DATA_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function uses the nearest GPS receiver to the location of the
//	telescope to find the STEC value of the direction specified by
//	using the single, nearest GPS satellite on the sky.


//_FILE  files used and logical units used

//_LIMS  design limitations
//	This function assumes that the telescope position and the receiver
//	position are close enough that the observation direction of the
//	telescope is the same as that for the receiver.

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 02  James M Anderson  --JIVE  start
//      2007 Mar 09  JMA  --return error estimate
//	2007 Apr 17  JMA  --convert to new STEC/SRM return struct

//_END

{
    GPS_receiver* rec = get_nearest_receiver(telescope);
    if(rec == NULL) return GPS_collection_return(GPS_BAD_DATA_VALUE);
    GPS_receiver& receiver = *rec;

    Space_Vector target_pierce =
            telescope.get_pierce_location(target,
                                          selection_criteria.Default_Iono_Height,
                                          radius_Earth);
    

    // Ok, find the starting and the ending time
    Uint32 start_index
        = time_arr->get_nearest_index_2(MJD_obs
                                        - selection_criteria.Averaging_Time_Half_Width
                                        * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 end_index
        = time_arr->get_nearest_index(MJD_obs
                                      + selection_criteria.Averaging_Time_Half_Width
                                      * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Real64 VTEC_sum = 0.0;
    Real64 weight_sum = 0.0;
    for(Uint32 t=start_index; t <= end_index; t++) {
        // for this time, find the closest satellite which
        // meets our criteria
        Uint32 best_satellite = receiver.station_obs.NUM_SATELLITES;
        Real64 best_sat_dist = 1E300;
        // Run through the list of satellites
        for(Uint32 s=0; s < receiver.station_obs.NUM_SATELLITES; s++) {
            // get this satellite
            Sint16 sat_num;
            Real64 STEC;
            Real64 sigma;
            receiver.station_obs.get_data(t, s, &sat_num, &STEC, &sigma);
            if(sat_num < 0) continue;
            if((STEC == GPS_BAD_DATA_VALUE) || (sigma == GPS_BAD_DATA_VALUE))
                continue;
            Space_Unit_Vector satellite =
                (sat_pos->get_sat(t, Uint32(sat_num))
                 - receiver.station_position).make_unit_vector();

            Real64 angle = target.Angular_Separation(satellite);
            LatLon_Cart AltAz = receiver.station_position.convert_RADec_to_AltAz(satellite);
            
            if(( test_satellite_criteria_fail(target_pierce,
                                              receiver.station_position,
                                              satellite,
                                              angle, AltAz.Lat() ) )) 
                continue;
            // Ok, this is a good satellite.  Calculate the angular distance
            if(angle < best_sat_dist) {
                best_sat_dist = angle;
                best_satellite = s;
            }
        }
        // Did we find one?
        if(best_satellite == receiver.station_obs.NUM_SATELLITES) continue;
        // Add this one in to the average
        {
            Sint16 sat_num;
            Real64 STEC;
            Real64 sigma;
            receiver.station_obs.get_data(t, best_satellite,
                                          &sat_num, &STEC, &sigma);
            Space_Unit_Vector satellite =
                (sat_pos->get_sat(t, Uint32(sat_num))
                 - receiver.station_position).make_unit_vector();
            LatLon_Cart AltAz = receiver.station_position.convert_RADec_to_AltAz(satellite);
            Real64 El   = AltAz.Lat();
            Real64 VTEC_factor = get_simple_VTEC_scaling(El,
                                                         receiver.station_position.Elevation(),
                                                         selection_criteria.Default_Iono_Height);
            if(sigma <= 0.0) sigma = STEC;
            Real64 weight = 1.0 / sigma;
            VTEC_sum += STEC * VTEC_factor * weight;
            weight_sum += weight;
        }
    } // for t over the number of time indices

    // Now form the average, and convert to the position of the target
    if(weight_sum != 0.0) {
        Real64 VTEC = VTEC_sum / weight_sum;
        LatLon_Cart AltAz = telescope.convert_RADec_to_AltAz(target);
        Real64 El   = AltAz.Lat();
        if(El <= 0.0) return GPS_BAD_DATA_VALUE;
        Real64 VTEC_factor = get_simple_VTEC_scaling(El, telescope.Elevation(),
                                                     selection_criteria.Default_Iono_Height);
        Real64 error_estimate =
            (end_index - start_index +1) / weight_sum/ VTEC_factor;
        return GPS_collection_return((VTEC / VTEC_factor),
                                     error_estimate,
                                     GPS_BAD_DATA_VALUE,
                                     GPS_BAD_DATA_VALUE);
    }
    return GPS_collection_return(GPS_BAD_DATA_VALUE);
}
    

















//_TITLE  get_single_receiver_average_STEC --
GPS_collection_return GPS_collection::get_single_receiver_average_STEC(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs       // I  The time of the observation, as an MJD.
    )
// GPS_collection_return GPS_collection::get_single_receiver_average_STEC
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.
//                                   The SRM and sigma_SRM are always returned as
//                                   GPS_BAD_DATA_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function uses the nearest GPS receiver to the location of the
//	telescope to find the STEC value of the direction specified by
//	using all of the GPS satellite on the sky (within the specified limits).


//_FILE  files used and logical units used

//_LIMS  design limitations
//	This function assumes that the telescope position and the receiver
//	position are close enough that the observation direction of the
//	telescope is the same as that for the receiver.

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 02  James M Anderson  --JIVE  start
//      2007 Mar 09  JMA  --return error estimate
//	2007 Apr 17  JMA  --convert to new STEC/SRM return struct

//_END

{
    GPS_receiver& receiver = *(get_nearest_receiver(telescope));

    Space_Vector target_pierce =
            telescope.get_pierce_location(target,
                                          selection_criteria.Default_Iono_Height,
                                          radius_Earth);

    // Ok, find the starting and the ending time
    Uint32 start_index
        = time_arr->get_nearest_index_2(MJD_obs
                                        - selection_criteria.Averaging_Time_Half_Width
                                        * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 end_index
        = time_arr->get_nearest_index(MJD_obs
                                      + selection_criteria.Averaging_Time_Half_Width
                                      * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Real64 VTEC_sum = 0.0;
    Real64 weight_sum = 0.0;
    for(Uint32 t=start_index; t <= end_index; t++) {
        // for this time, find all satellites which
        // meet our criteria

        // Run through the list of satellites
        for(Uint32 s=0; s < receiver.station_obs.NUM_SATELLITES; s++) {
            // get this satellite
            Sint16 sat_num;
            Real64 STEC;
            Real64 sigma;
            receiver.station_obs.get_data(t, s, &sat_num, &STEC, &sigma);
            if(sat_num < 0) continue;
            if((STEC == GPS_BAD_DATA_VALUE) || (sigma == GPS_BAD_DATA_VALUE))
                continue;
            Space_Unit_Vector satellite =
                (sat_pos->get_sat(t, Uint32(sat_num))
                 - receiver.station_position).make_unit_vector();

            Real64 angle = target.Angular_Separation(satellite);
            LatLon_Cart AltAz = receiver.station_position.convert_RADec_to_AltAz(satellite);
            Real64 El   = AltAz.Lat();
            
            if(( test_satellite_criteria_fail(target_pierce,
                                              receiver.station_position,
                                              satellite,
                                              angle, El ) )) 
                continue;
            // Ok, this is a good satellite.
            // Add this one in to the average
            Real64 VTEC_factor = get_simple_VTEC_scaling(El,
                                                         receiver.station_position.Elevation(),
                                                         selection_criteria.Default_Iono_Height);
            if(sigma <= 0.0) sigma = 1.0;
            Real64 weight = 1.0 / sigma;
            VTEC_sum += STEC * VTEC_factor * weight;
            weight_sum += weight;
        } // for s over all satellites
    } // for t over the number of time indices

    // Now form the average, and convert to the position of the target
    if(weight_sum != 0.0) {
        Real64 VTEC = VTEC_sum / weight_sum;
        LatLon_Cart AltAz = telescope.convert_RADec_to_AltAz(target);
        Real64 El   = AltAz.Lat();
        if(El <= 0.0) return GPS_BAD_DATA_VALUE;
        Real64 VTEC_factor = get_simple_VTEC_scaling(El, telescope.Elevation(),
                                                     selection_criteria.Default_Iono_Height);
        Real64 error_estimate =
            (end_index - start_index +1) / weight_sum/ VTEC_factor;
        return GPS_collection_return((VTEC / VTEC_factor),
                                     error_estimate,
                                     GPS_BAD_DATA_VALUE,
                                     GPS_BAD_DATA_VALUE);
    }
    return GPS_collection_return(GPS_BAD_DATA_VALUE);
}












Uint32 GPS_collection::get_fake_sat_num(const Uint32 sat_ID,
                                        Uint32& num_sats) throw()
{
    assert(num_sats <= GPS_MAX_POSSIBLE_SATELLITES);
    Uint32 i;
    for(i=0; i < num_sats; i++) {
        if(satellite_workspace[i] == sat_ID) return i;
    }
    satellite_workspace[i] = sat_ID;
    num_sats++;
    return i;
}

void GPS_collection::clear_fake_sat_num() throw()
{
    for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++) {
        satellite_workspace[s] = UINT32_MAX;
    }
    return;
}





Uint32 GPS_collection::get_fake_track_num(const Uint32 rec_ID, // real Rec
                                          const Sint16 sat_ID,
                                          const Sint16 track_ID,
                                          Uint32& num_tracks) throw()
{
    assert(num_tracks <= Satellite_Track_Max_Tracks);
    Uint32 real_track_number = track_ID + 
        satellite_track_main_ID[rec_ID*GPS_MAX_POSSIBLE_SATELLITES + sat_ID];
    Uint32 i;
    for(i=0; i < num_tracks; i++) {
        if(satellite_track_track_pos[i] == real_track_number) return i;
    }
    assert(i < Satellite_Track_Max_Tracks);
    satellite_track_reciever_pos[i] = rec_ID;
    satellite_track_satellite_pos[i] = sat_ID;
    satellite_track_track_pos[i] = real_track_number;
    num_tracks++;
    return i;
}

void GPS_collection::clear_fake_track_num() throw()
{
    for(Uint32 s=0; s < Satellite_Track_Max_Tracks; s++) {
        satellite_track_track_pos[s] = UINT32_MAX;
    }
    return;
}

Uint32 GPS_collection::get_fake_track_max(void) const throw()
{
    Uint32 i;
    for(i=0; i < Satellite_Track_Max_Tracks; i++) {
        if(satellite_track_track_pos[i] == UINT32_MAX) break;
    }
    return i;
}

void GPS_collection::get_real_track_info(const Uint32 fake_track_num,
                                         Uint32& rec_ID,
                                         Sint16& sat_ID,
                                         Sint16& track_ID,
                                         Uint32& real_track_num) const throw()
{
    assert(fake_track_num < Satellite_Track_Max_Tracks);
    real_track_num = satellite_track_track_pos[fake_track_num];
    rec_ID = satellite_track_reciever_pos[fake_track_num];
    sat_ID = satellite_track_satellite_pos[fake_track_num];
    track_ID = real_track_num
        - satellite_track_main_ID[rec_ID*GPS_MAX_POSSIBLE_SATELLITES + sat_ID];
    return;
}
    









//_TITLE  get_multiple_receiver_2D_MIM_fit
GPS_collection_return GPS_collection::get_multiple_receiver_2D_MIM_fit(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs,      // I  The time of the observation, as an MJD.
    const GPS_Fit_Type_Enum fit_type,
                               // I  Do a simple or time dependance
    const bool Fit_Station_Bias,//I  Fit or not the station biases
    const bool Force_Zero_Mean_Bias,// I  Should the bias level be forced to 0?
    const bool Hold_Valid_Station_Bias,//I  Hold or not receivers with valid bias
    const bool Fit_Object_Bias,// I  Fit or not the object(satellite) biases
    const GPS_receiver* const Central_Receiver_Only
                               // I  If this is not NULL, then allow this
                               //    and only this receiver to have its bias
                               //    level vary and be fit.
    )
// GPS_collection_return GPS_collection::get_multiple_receiver_2D_MIM_fit
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function uses nearby (hopefully many more than one) GPS receivers
//	to calculate a 2-D MIM fit to the ionosphere.  It then calculates
//	the model prediction for the telescope line of sight to the target.

//	This function calls fit_simple_linear_model_2 from the linfit.cxx area.
//	This routine will do the actual fitting.  Hopefully the user
//	has set up a good ionosphere height for the 2-D fitting process.


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls
//	fit_simple_linear_model_2
//	apply_simple_linear_model_2

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 04  James M Anderson  --JIVE  start
//      2007 Mar 09  JMA  --return error estimate
//	2007 Apr 17  JMA  --convert to new STEC/SRM return struct

//_END

{
    // Get the list of nearby receivers
    fprintf(stderr, "in GPS_collection_return GPS_collection::get_multiple_receiver_2D_MIM_fit with fit_type %u\n", fit_type);
    GPS_receiver** Receiver_List = NULL;
    Uint32 NUM_RECEIVER = 0;
    get_nearby_receivers(telescope, Receiver_List, NUM_RECEIVER);
    fprintf(stderr, "Doing 2-D fit with %u receivers\n", NUM_RECEIVER);
    if(NUM_RECEIVER == 0) return GPS_collection_return(GPS_BAD_DATA_VALUE);

    Space_Vector target_pierce =
            telescope.get_pierce_location(target,
                                          selection_criteria.Default_Iono_Height,
                                          radius_Earth);

    // Ok, find the starting and the ending time
    Uint32 start_index
        = time_arr->get_nearest_index_2(MJD_obs
                                        - selection_criteria.Averaging_Time_Half_Width
                                        * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 end_index
        = time_arr->get_nearest_index(MJD_obs
                                      + selection_criteria.Averaging_Time_Half_Width
                                      * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 time_increment = 1;
    if(Fit_Station_Bias) {
        if(end_index - start_index > 30) 
            time_increment = (end_index - start_index) / 30;
    }

    // Now figure out how many observations there are.
    Uint32 Num_Obs = 0;
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            // Run through the list of satellites
            Num_Obs += Receiver_List[r]->station_obs.NUM_SATELLITES;
        }
    }
    const Uint32 MAX_NUM_ACTUAL_PARAMETERS =
        selection_criteria.MAX_NUM_PARAMETERS
        +( ((Fit_Station_Bias))? (GPS_MAX_POSSIBLE_RECEIVER_TYPE*NUM_RECEIVER):0)
        +( ((Fit_Object_Bias))? (GPS_MAX_POSSIBLE_SATELLITES):0);
    Uint32 Num_Work = 0;
    switch(fit_type) {
        case multiple_2D :
        case multiple_2D_time :
            Num_Work = (Num_Obs+1+GPS_MAX_POSSIBLE_RECEIVER_TYPE*NUM_RECEIVER)
                * (2*MAX_NUM_ACTUAL_PARAMETERS+4)
                + MAX_NUM_ACTUAL_PARAMETERS*(MAX_NUM_ACTUAL_PARAMETERS+3)
                + selection_criteria.NUM_TIME_TERMS;
            break;
        case multiple_2D_grad :
        case multiple_2D_timg :
            Num_Work = Num_Obs*(2*selection_criteria.MAX_NUM_PARAMETERS+7)
                + MAX_NUM_ACTUAL_PARAMETERS*7
                + selection_criteria.NUM_TIME_TERMS;
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type), __FILE__, __LINE__, __func__);
            return GPS_BAD_DATA_VALUE;
    }
    reallocate_2D_observations(Num_Obs, Num_Work);

    std::vector<char> receiver_present(GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    Ionosphere_Base* const model_ionosphere =
        generate_new_theoretical_ionosphere(selection_criteria.Theo_Model_Type,
                                            GPS_DEFAULT_MODEL_ELECTRON_PRECISION,
                                            GPS_DEFAULT_MODEL_FARADAY_PRECISION);

    // Now add everything to the observation list
    Uint32 obs = 0;
    Uint32 num_sats = 0; clear_fake_sat_num();
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        const Real64 MJD = time_arr->get_time(t);
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                s++, obs++) {
                use_flag[obs] = true;
                observations_2D[obs].station_number = r;
                
                // get this satellite
                Sint16 sat_num;
                Real64 STEC;
                Real64 sigma;
                Receiver_List[r]->station_obs.get_data(t, s, &sat_num,
                                                       &STEC, &sigma);
                if(sat_num < 0) use_flag[obs] = false;
                else {
                    observations_2D[obs].MJD = MJD;
                    Uint32 receiver_type = sat_num/100;
                    if(Central_Receiver_Only == NULL) {
                        Sint32 bias_valid =
                            Receiver_List[r]->station_obs.get_bias_valid(receiver_type);
                        if( ((bias_valid)) && ((Hold_Valid_Station_Bias)) ) {
                            observations_2D[obs].station_receiver_id =UINT32_MAX;
                        }
                        else {
                            receiver_present[receiver_type] = 1;
                            observations_2D[obs].station_receiver_id =
                                r + receiver_type*NUM_RECEIVER;
                        }
                    }
                    else if(Receiver_List[r] == Central_Receiver_Only) {
                        // just allow this one to be fit
                        receiver_present[receiver_type] = 1;
                        observations_2D[obs].station_receiver_id =
                            r + receiver_type*NUM_RECEIVER;
                    }
                    else {
                        // Only doing central receiver, but this one is not it
                        observations_2D[obs].station_receiver_id = UINT32_MAX;
                    }
                        
                    // get the fake sat number
                    Uint32 fake_sat_num = get_fake_sat_num(sat_num, num_sats);
                    observations_2D[obs].object_id = fake_sat_num;
                    observations_2D[obs].object_number = sat_num;
                    observations_2D[obs].STEC = STEC;
                    observations_2D[obs].sigma_STEC = sigma;
                    if((STEC == GPS_BAD_DATA_VALUE)
                       || (sigma == GPS_BAD_DATA_VALUE)) use_flag[obs] = false;
                    Space_Unit_Vector satellite =
                        (sat_pos->get_sat(t, Uint32(sat_num))
                         - Receiver_List[r]->station_position).make_unit_vector();

                    Real64 angle = target.Angular_Separation(satellite);
                    LatLon_Cart AltAz =
                        Receiver_List[r]->station_position.convert_RADec_to_AltAz(satellite);
                    Real64 El   = observations_2D[obs].El = AltAz.Lat();
                    observations_2D[obs].Az = AltAz.Lon();
                    
            
                    if(( test_satellite_criteria_fail(target_pierce,
                                                      Receiver_List[r]->station_position,
                                                      satellite,
                                                      angle, El ) )) {
                        use_flag[obs] = false;
                    }
                    else {
                        observations_2D[obs].pierce_point =
                            Receiver_List[r]->station_position.get_pierce_location(
                                satellite,
                                selection_criteria.Default_Iono_Height,
                                radius_Earth);
                        // Ok, this is a good satellite.
                        // Add this one in to the average
                        Real64 VTEC_factor = get_simple_VTEC_scaling(
                            El, Receiver_List[r]->station_position.Elevation(),
                            selection_criteria.Default_Iono_Height);
                        observations_2D[obs].VTEC = STEC * VTEC_factor;
                        // conceptually, this should be sigma * VTEC_factor,
                        // but I don't want to trust low elevations that well.
                        observations_2D[obs].sigma_VTEC = sigma;
                        if((model_ionosphere)) {
                            observations_2D[obs].model_VTEC =
                                get_receiver_obs_model_STEC(model_ionosphere,
                                                            Receiver_List[r],
                                                            MJD,
                                                            satellite, t, s);
                            if(!finite(observations_2D[obs].model_VTEC)) {
                                fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
                                exit(1);
                            }
                            if(observations_2D[obs].model_VTEC
                               != GPS_BAD_DATA_VALUE)
                                observations_2D[obs].sigma_model_VTEC = 0.0;
                            else observations_2D[obs].sigma_model_VTEC = 1.0E24;
                            observations_2D[obs].model_VTEC *= VTEC_factor;
                        }
                        else {
                            observations_2D[obs].model_VTEC = 0.0;
                            observations_2D[obs].sigma_model_VTEC = -1.0;
                        }
#ifdef DEBUG                        
                        fprintf(stderr, "Got additional point %4u %2u %3u %2u at %10.3E %9.1E %10.3E    %9.3f  %9.3f\n", t, r, observations_2D[obs].station_receiver_id, observations_2D[obs].object_id, observations_2D[obs].VTEC, sigma, observations_2D[obs].model_VTEC,El*M_RAD2DEG, observations_2D[obs].Az*M_RAD2DEG);
#endif
                    }
                } // no sat avialable
            } // for s over all satellites
        } // for r over receivers
    } // for t over the number of time indices

    // Ok, how many of these are actually good?
    Uint32 good_obs = 0;
    for(obs=0; obs < Num_Obs; obs++) if(use_flag[obs]) good_obs++;
    if(good_obs == 0) {
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }

    // How many different receiver types are there?
    Uint32 num_receiver_types;
    for(num_receiver_types = GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        num_receiver_types > 0; num_receiver_types--) {
        if((receiver_present[num_receiver_types-1])) break;
    }
    fprintf(stderr, "Found %u receiver types\n", num_receiver_types);

    Uint32 Num_Use_Param = (selection_criteria.MAX_NUM_PARAMETERS < good_obs) ?
        selection_criteria.MAX_NUM_PARAMETERS : good_obs;
    if(Num_Use_Param > 4*NUM_RECEIVER) Num_Use_Param = 4*NUM_RECEIVER;
    Uint32 NUM_POLY = Num_Use_Param;
    if((Fit_Station_Bias)) Num_Use_Param += NUM_RECEIVER*num_receiver_types;
    if((Fit_Object_Bias)) Num_Use_Param += num_sats;

    // Now for the actual fit
    Real64 std_dev = 0.0;
    Sint32 return_code = 0;
    // Clear the parameter space
    for(Uint32 p=0; p < Num_Use_Param; p++) workspace[p] = 0.0;
    // If using a model ionosphere, set that parameter to 1
    if((model_ionosphere)) workspace[NUM_POLY-1] = 1.0;


    
    switch(fit_type) {
        case multiple_2D :
            return_code = fit_simple_linear_model_2(
                Num_Use_Param,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_2D,
                std_dev,
                workspace
                );
            break;
        case multiple_2D_time :
            return_code = fit_2D_time_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_TIME_TERMS,
                MJD_obs,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_2D,
                std_dev,
                workspace
                );
            break;
        case multiple_2D_grad :
            return_code = fit_simple_linear_model_2_grad(
                Num_Use_Param,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace + Num_Use_Param,
                observations_2D,
                std_dev,
                workspace
                );
            break;
        case multiple_2D_timg :
            return_code = fit_simple_linear_model_2_grad(
                Num_Use_Param,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace + Num_Use_Param,
                observations_2D,
                std_dev,
                workspace
                );
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            if((model_ionosphere)) delete model_ionosphere;
            return GPS_BAD_DATA_VALUE;
    }
    if((return_code)) {
        // something bad happened
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }


    // If we are fitting for station biases, then compute the error
    // for individual stations
    if(Fit_Station_Bias) {
        const Uint32 NUM_ITEMS = NUM_RECEIVER*num_receiver_types;
        Real64* sum_array = workspace + Max_Number_Workspace
            - 3*NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        Real64* sum_sqr_array = sum_array + NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        Real64* count_array = sum_sqr_array + NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        for(Uint32 i=0; i < 3*NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE; i++) {
            sum_array[i] = 0.0;
        }

        obs = 0;
        for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
            for(Uint32 r=0; r < NUM_RECEIVER; r++) {
                for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                    s++, obs++) {
                    if(use_flag[obs] == false) continue;
                    Uint32 station_id = observations_2D[obs].station_receiver_id;
                    if(station_id == UINT32_MAX) continue;
                    Real64 Delta_STEC = observations_2D[obs].STEC
                        - observations_2D[obs].model_VTEC *
                        (observations_2D[obs].STEC / observations_2D[obs].VTEC);
                    sum_array[station_id] += Delta_STEC;
                    sum_sqr_array[station_id] += Delta_STEC * Delta_STEC;
                    count_array[station_id] += 1.0;
                } // for s over all satellites
            } // for r over receivers
        } // for t over the number of time indices
        for(Uint32 i=0; i < NUM_ITEMS; i++) {
            if(count_array[i] > 4.0) {
                if(sum_sqr_array[i] != 0.0) {
                    Real64 var = ( sum_sqr_array[i]
                                   - sum_array[i] * sum_array[i] / count_array[i])
                        / (count_array[i]-1.0);
                    if(var < 0.0) var = 0.0;
                    Real64 std_dev = sqrt(var);
                    sum_array[i] = std_dev;
                    //printf("For station %4u got residual level %E\n", i, std_dev);
                }
            }
            else sum_array[i] = 0.0;
        }
    } // if placing station error levels into workspace
    if(Fit_Object_Bias) {
        for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++)
            satellite_bias_workspace[s] = 0.0;
        for(Uint32 s=0; s < num_sats; s++) {
            Uint32 sat_num = satellite_workspace[s];
            satellite_bias_workspace[sat_num] =
                workspace[Num_Use_Param - num_sats + s];
        }
    }

    
    // Now apply to the target
    Real64 VTEC_factor = get_simple_VTEC_scaling(
        observations_2D[0].El, telescope.Elevation(),
        selection_criteria.Default_Iono_Height);
    use_flag[0] = true;
    observations_2D[0].station_number = UINT32_MAX;
    observations_2D[0].station_receiver_id = UINT32_MAX;
    observations_2D[0].object_id = UINT32_MAX;
    observations_2D[0].object_number = UINT32_MAX;
    LatLon_Cart AltAz = telescope.convert_RADec_to_AltAz(target);
    observations_2D[0].El = AltAz.Lat();
    observations_2D[0].Az = AltAz.Lon();
    observations_2D[0].MJD = MJD_obs;
    observations_2D[0].pierce_point = target_pierce;
    Real64 model_STEC = GPS_BAD_DATA_VALUE;
    Real64 model_STEC_err = GPS_BAD_DATA_VALUE;
    Real64 model_SRM = GPS_BAD_DATA_VALUE;
    if((model_ionosphere)) {
        fprintf(stdout, "In GPS_collection::calling model_ionosphere->Integrate_Electron_Faraday\n");
        model_ionosphere->Integrate_Electron_Faraday(
            telescope,
            MJD_obs,
            target,
            &model_STEC,
            &model_STEC_err,
            &model_SRM,
            telescope.Elevation());
        if(!finite(model_STEC)) {
            fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
            exit(1);
        }
        observations_2D[0].model_VTEC = model_STEC * VTEC_factor;
        observations_2D[0].sigma_model_VTEC = 0.0;
    }
    else {
        observations_2D[0].model_VTEC = 0.0;
        observations_2D[0].sigma_model_VTEC = -1.0;
    }

    if((model_ionosphere)) {
        delete model_ionosphere;
    }
    
    
    switch(fit_type) {
        case multiple_2D :
        case multiple_2D_grad :
            apply_simple_linear_model_2(
                Num_Use_Param,
                telescope,
                1,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_2D
                );
            break;
        case multiple_2D_time :
        case multiple_2D_timg :
            apply_2D_time_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_TIME_TERMS,
                MJD_obs,
                telescope,
                1,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_2D
                );
            break;
            
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }


//#ifdef DEBUG
    fprintf(stderr, "get_multiple_receiver_2D_MIM_fit: Got fit value %12.3E %10.1E     %9.3f  %9.3f at %12.5f\n", observations_2D[0].model_VTEC, observations_2D[0].sigma_model_VTEC, observations_2D[0].El*M_RAD2DEG, observations_2D[0].Az*M_RAD2DEG, MJD_obs);
//#endif


    if(observations_2D[0].sigma_model_VTEC < 0.0)
        return GPS_collection_return(GPS_BAD_DATA_VALUE);

    Real64 error_estimate = std_dev / VTEC_factor;
    Real64 sigma_SRM = GPS_BAD_DATA_VALUE;
    if(model_SRM != GPS_BAD_DATA_VALUE) {
        sigma_SRM = error_estimate * model_SRM / model_STEC;
        model_SRM *= workspace[NUM_POLY-1];
    }

//    fprintf(stdout, "In GPS_collection::returning!!\n");
    return GPS_collection_return(observations_2D[0].model_VTEC / VTEC_factor,
                                 error_estimate,
                                 model_SRM,
                                 sigma_SRM);
}






//_TITLE  get_multiple_receiver_3D_MIM_fit
GPS_collection_return GPS_collection::get_multiple_receiver_3D_MIM_fit(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs,      // I  The time of the observation, as an MJD.
    const GPS_Fit_Type_Enum fit_type,
                               // I  Do a multi or a many-layer fit
    const bool use_IRI_data,   // I  Use or not an updated IRI ionosphere for the
                               //    3-D height scaling information
    const bool Fit_Station_Bias,//I  Fit or not the station biases
    const bool Force_Zero_Mean_Bias,// I  Should the bias level be forced to 0?
    const bool Hold_Valid_Station_Bias,//I  Hold or not receivers with valid bias
    const bool Fit_Object_Bias,// I  Fit or not the object(satellite) biases
    const GPS_receiver* const Central_Receiver_Only
                               // I  If this is not NULL, then allow this
                               //    and only this receiver to have its bias
                               //    level vary and be fit.
        )
// GPS_collection_return GPS_collection::get_multiple_receiver_3D_MIM_fit
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function uses nearby (hopefully many more than one) GPS receivers
//	to calculate a 3-D MIM fit to the ionosphere.  It then calculates
//	the model prediction for the telescope line of sight to the target.

//	The 3-D MIM model currently has 2 options.  The multilayer model
//	assumes that the latitude and longitude variation across the ionosphere
//	is relatively the same at all heights above the Earth.  Multiple
//	layers are used to model the ionosphere with multiple pierce points
//	through a simple 2-D structure.

//	The manylayer model, in contrast, assumes that each height layer
//	can have a different latitude and longitude dependence.  This allows
//	for greater variation, but more parameters are needed to describe the
//	variation at the different heights.

//	At the time of 2007 Jan 04, these two different models seem to produce
//	roughly equivalent results for about the same number of parameters.
//	It is not clear that either way is better than the other at the moment.


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls
//	fit_multilayer_linear_model_2
//	apply_multilayer_linear_model_2
//	fit_manylayer_linear_model_2
//	apply_manylayer_linear_model_2

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 04  James M Anderson  --JIVE  start
//	2007 Feb 28  JMA  --add code for bias calculations
//      2007 Mar 09  JMA  --return error estimate
//	2007 Apr 17  JMA  --convert to new STEC/SRM return struct

//_END

{
    fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit\n");
    // Get the list of nearby receivers
    GPS_receiver** Receiver_List = NULL;
    Uint32 NUM_RECEIVER = 0;
    get_nearby_receivers(telescope, Receiver_List, NUM_RECEIVER);
    fprintf(stdout, "number of receivers %d\n", NUM_RECEIVER);
    if(NUM_RECEIVER == 0) return GPS_collection_return(GPS_BAD_DATA_VALUE);
    fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit passed if(NUM_RECEIVER == 0) test\n");

    Space_Vector target_pierce =
            telescope.get_pierce_location(target,
                                          selection_criteria.Default_Iono_Height,
                                          radius_Earth);

// This following stuff seems to cause segmentation faults with Scientific Linux.
// Tests with RI_G03/RI_G05 show that it seems to do nothing useful anyway so 
// I'm commenting the whole thing out
/*
    if(use_IRI_data) {
        fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit inside if(use_IRI_data)\n");
        // Ok, update the height profile with the IRI data at the target
        // pierce point
        Station_LatLon t(target_pierce);
        JMA_VEX_AREA::VEX_Time v(JMA_VEX_AREA::MJD_OFFSET, MJD_obs);
        struct tm eval_time = v.C_tm_time();
        fill_IRI_electron_densities(t, eval_time);
    }
*/

    // Ok, find the starting and the ending time
    Uint32 start_index
        = time_arr->get_nearest_index_2(MJD_obs
                                        - selection_criteria.Averaging_Time_Half_Width
                                        * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 end_index
        = time_arr->get_nearest_index(MJD_obs
                                      + selection_criteria.Averaging_Time_Half_Width
                                      * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 time_increment = 1;
    if(Fit_Station_Bias) {
        if(end_index - start_index > 30) 
            time_increment = (end_index - start_index) / 30;
    }

    // Now figure out how many observations there are.
    Uint32 Num_Obs = 0;
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            // Run through the list of satellites
            Num_Obs += Receiver_List[r]->station_obs.NUM_SATELLITES;
        }
    }
    const Uint32 MAX_NUM_ACTUAL_PARAMETERS =
        selection_criteria.MAX_NUM_PARAMETERS
        +( ((Fit_Station_Bias))? (GPS_MAX_POSSIBLE_RECEIVER_TYPE*NUM_RECEIVER):0)
        +( ((Fit_Object_Bias))? (GPS_MAX_POSSIBLE_SATELLITES):0);
    Uint32 Num_Work = 0;
    switch(fit_type) {
        case multiple_3D_mult :
        case multiple_3D_many :
        case multiple_3D_sphe :
            Num_Work =
                (Num_Obs + 1 + GPS_MAX_POSSIBLE_RECEIVER_TYPE*NUM_RECEIVER)
                * (2*MAX_NUM_ACTUAL_PARAMETERS+4)
                + MAX_NUM_ACTUAL_PARAMETERS
                * (MAX_NUM_ACTUAL_PARAMETERS+3)
                + selection_criteria.NUM_HEIGHTS * 5;
            break;
        case multiple_3D_sphg:
            Num_Work = Num_Obs*(2*selection_criteria.MAX_NUM_PARAMETERS+7)
                + MAX_NUM_ACTUAL_PARAMETERS*7
                + selection_criteria.NUM_HEIGHTS * 4;
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type), __FILE__, __LINE__, __func__);
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    reallocate_3D_observations(Num_Obs, Num_Work);

    std::vector<char> receiver_present(GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    fprintf(stdout, "In GPS_collection::calling generate_new_theoretical_ionosphere\n");
    Ionosphere_Base* const model_ionosphere =
        generate_new_theoretical_ionosphere(selection_criteria.Theo_Model_Type,
                                            GPS_DEFAULT_MODEL_ELECTRON_PRECISION,
                                            GPS_DEFAULT_MODEL_FARADAY_PRECISION);

    // Now add everything to the observation list
    Uint32 obs = 0;
    Uint32 num_sats = 0; clear_fake_sat_num();
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        const Real64 MJD = time_arr->get_time(t);
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                s++, obs++) {
                use_flag[obs] = true;
                observations_3D[obs].station_number = r;
                observations_3D[obs].station =
                    &(Receiver_List[r]->station_position);
                
                // get this satellite
                Sint16 sat_num;
                Real64 STEC;
                Real64 sigma;
                Receiver_List[r]->station_obs.get_data(t, s, &sat_num,
                                                       &STEC, &sigma);
                if(sat_num < 0) use_flag[obs] = false;
                else {
                    observations_3D[obs].MJD = MJD;
                    Uint32 receiver_type = sat_num/100;
                    if(Central_Receiver_Only == NULL) {
                        Sint32 bias_valid =
                            Receiver_List[r]->station_obs.get_bias_valid(receiver_type);
                        if( ((bias_valid)) && ((Hold_Valid_Station_Bias)) ) {
                            observations_3D[obs].station_receiver_id =UINT32_MAX;
                        }
                        else {
                            receiver_present[receiver_type] = 1;
                            observations_3D[obs].station_receiver_id =
                                r + receiver_type*NUM_RECEIVER;
                        }
                    }
                    else if(Receiver_List[r] == Central_Receiver_Only) {
                        // just allow this one to be fit
                        receiver_present[receiver_type] = 1;
                        observations_3D[obs].station_receiver_id =
                            r + receiver_type*NUM_RECEIVER;
                    }
                    else {
                        // Only doing central receiver, but this one is not it
                        observations_3D[obs].station_receiver_id = UINT32_MAX;
                    }
                    Uint32 fake_sat_num = get_fake_sat_num(sat_num, num_sats);
                    observations_3D[obs].object_id = fake_sat_num;
                    observations_3D[obs].object_number = sat_num;
                    observations_3D[obs].STEC = STEC;
                    observations_3D[obs].sigma_STEC = sigma;
                    if((STEC == GPS_BAD_DATA_VALUE)
                       || (sigma == GPS_BAD_DATA_VALUE)) use_flag[obs] = false;
                    Space_Unit_Vector satellite =
                        (sat_pos->get_sat(t, Uint32(sat_num))
                         - Receiver_List[r]->station_position).make_unit_vector();
                    observations_3D[obs].direction = satellite;

                    Real64 angle = target.Angular_Separation(satellite);
                    LatLon_Cart AltAz =
                        Receiver_List[r]->station_position.convert_RADec_to_AltAz(satellite);
                    Real64 El   = observations_3D[obs].El = AltAz.Lat();
                    /*Real64 Az   =*/ observations_3D[obs].Az = AltAz.Lon();
            
                    if(( test_satellite_criteria_fail(target_pierce,
                                                      Receiver_List[r]->station_position,
                                                      satellite,
                                                      angle, El ) )) {
                        use_flag[obs] = false;
                    }
                    else {
                        if((model_ionosphere)) {
                            observations_3D[obs].model_STEC =
                                get_receiver_obs_model_STEC(model_ionosphere,
                                                            Receiver_List[r],
                                                            MJD,
                                                            satellite, t, s);
                            if(!finite(observations_3D[obs].model_STEC)) {
                                fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
                                exit(1);
                            }
                            if(observations_3D[obs].model_STEC
                               != GPS_BAD_DATA_VALUE)
                                observations_3D[obs].sigma_model_STEC = 0.0;
                            else observations_3D[obs].sigma_model_STEC = 1.0E24;
                        }
                        else {
                            observations_3D[obs].model_STEC = 0.0;
                            observations_3D[obs].sigma_model_STEC = -1.0;
                        }
#ifdef DEBUG                        
                        fprintf(stderr, "Got %12.5f %6u %3u %3u at %10.3E %9.1E %10.3E    %9.3f  %9.3f\n", time_arr->get_time(t), obs, observations_3D[obs].station_number, observations_3D[obs].object_number, observations_3D[obs].STEC, sigma, observations_3D[obs].model_STEC,El*M_RAD2DEG, observations_3D[obs].Az*M_RAD2DEG);
#endif
                    }
                } // no sat avialable
            } // for s over all satellites
        } // for r over receivers
    } // for t over the number of time indices

    // Ok, how many of these are actually good?
    Uint32 good_obs = 0;
    for(obs=0; obs < Num_Obs; obs++) if(use_flag[obs]) good_obs++;
    if(good_obs == 0) {
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }

    // How many different receiver types are there?
    Uint32 num_receiver_types;
    for(num_receiver_types = GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        num_receiver_types > 0; num_receiver_types--) {
        if((receiver_present[num_receiver_types-1])) break;
    }
    fprintf(stderr, "Found %u receiver types\n", num_receiver_types);

    Uint32 Num_Use_Param = (selection_criteria.MAX_NUM_PARAMETERS < good_obs) ?
        selection_criteria.MAX_NUM_PARAMETERS : good_obs;
    if(Num_Use_Param > 3*NUM_RECEIVER) Num_Use_Param = 3*NUM_RECEIVER;
    Uint32 NUM_POLY = Num_Use_Param;
    if((Fit_Station_Bias)) Num_Use_Param += NUM_RECEIVER*num_receiver_types;
    if((Fit_Object_Bias)) Num_Use_Param += num_sats;

    // Now for the actual fit
    Real64 std_dev = 0.0;
    Sint32 return_code = 0;
    // Clear the parameter space
    for(Uint32 p=0; p < Num_Use_Param; p++) workspace[p] = 0.0;
    // If using a model ionosphere, set that parameter to 1
    if((model_ionosphere)) workspace[NUM_POLY-1] = 1.0;
    switch(fit_type) {
        case multiple_3D_mult :
            fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling fit_multilayer_linear_model_2\n");
            return_code= fit_multilayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_many :
            fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling fit_manylayer_linear_model_2\n");
            return_code= fit_manylayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_sphe :
            fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling fit_manylayer_spherical_h_model_2\n");
            return_code= fit_manylayer_spherical_h_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_sphg :
            fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling fit_manylayer_spherical_h_model_2_grad\n");
            return_code= fit_manylayer_spherical_h_model_2_grad(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                MJD_obs,
                Num_Obs,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            if((model_ionosphere)) delete model_ionosphere;
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    
    if((return_code)) {
        // something bad happened
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }


    // If we are fitting for station biases, then compute the error
    // for individual stations
    if(Fit_Station_Bias) {
        fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling Fit_Station_Bias = T\n");
        const Uint32 NUM_ITEMS = NUM_RECEIVER*num_receiver_types;
        Real64* sum_array = workspace + Max_Number_Workspace
            - 3*NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        Real64* sum_sqr_array = sum_array + NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        Real64* count_array = sum_sqr_array + NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE;
        for(Uint32 i=0; i < 3*NUM_RECEIVER*GPS_MAX_POSSIBLE_RECEIVER_TYPE; i++) {
            sum_array[i] = 0.0;
        }

        obs = 0;
        for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
            for(Uint32 r=0; r < NUM_RECEIVER; r++) {
                for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                    s++, obs++) {
                    if(use_flag[obs] == false) continue;
                    Uint32 station_id = observations_3D[obs].station_receiver_id;
                    if(station_id == UINT32_MAX) continue;
                    Real64 Delta_STEC = observations_3D[obs].STEC
                        - observations_3D[obs].model_STEC;
                    sum_array[station_id] += Delta_STEC;
                    sum_sqr_array[station_id] += Delta_STEC * Delta_STEC;
                    count_array[station_id] += 1.0;
                } // for s over all satellites
            } // for r over receivers
        } // for t over the number of time indices
        printf(" \n");
        printf("Information for fit time %.5f\n", MJD_obs);
        for(Uint32 i=0; i < NUM_ITEMS; i++) {
            if(count_array[i] > 4.0) {
                if(sum_sqr_array[i] != 0.0) {
                    Real64 var = ( sum_sqr_array[i]
                                   - sum_array[i] * sum_array[i] / count_array[i])
                        / (count_array[i]-1.0);
                    if(var < 0.0) var = 0.0;
                    Real64 std_dev = sqrt(var);
                    sum_array[i] = std_dev;
                    //printf("For station %4u got residual level %12.2E\n", i, std_dev);
                }
                else sum_array[i] = 0.0;
            }
            else sum_array[i] = 0.0;
        }
    } // if placing station error levels into workspace
    if(Fit_Object_Bias) {
        fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling Fit_Object_Bias = T\n");
        for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++)
            satellite_bias_workspace[s] = 0.0;
        for(Uint32 s=0; s < num_sats; s++) {
            Uint32 sat_num = satellite_workspace[s];
            satellite_bias_workspace[sat_num] =
                workspace[Num_Use_Param - num_sats + s];
        }
    }


//     if((Total_Bias_Correction_Done_Flag)) {
//         //FILE* fp = fopen("dump.out","a");
//         obs = 0;
//         for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
//             for(Uint32 r=0; r < NUM_RECEIVER; r++) {
//                 for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
//                     s++, obs++) {
//                     if(!use_flag[obs]) continue;
//                     if((observations_3D[obs].object_number != 8)
//                        && (observations_3D[obs].object_number != 16)
//                        && (observations_3D[obs].object_number != 24)) continue;
//                     //fprintf(fp, "%.5f %4.4s %4u %4u %10.3E %9.2E %8.1E %7.2f %6.2f\n",
//                     //        MJD_obs,
//                     //        Receiver_List[r]->station_name(),
//                     //        observations_3D[obs].station_number,
//                     //        observations_3D[obs].object_number,
//                     //        observations_3D[obs].STEC,
//                     //        observations_3D[obs].STEC - observations_3D[obs].model_STEC,
//                     //        observations_3D[obs].sigma_model_STEC,
//                     //        observations_3D[obs].Az*M_RAD2DEG,
//                     //        observations_3D[obs].El*M_RAD2DEG
//                     //        );
//                 } // for s over sats
//             } // for r over receivers
//         } // for t over times
//         fclose(fp);
//         fp = fopen("dump_p.out","a");
//         fprintf(fp, "%.5f  %10.2E %10.2E %10.2E\n",
//                 MJD_obs,
//                 workspace[0],
//                 workspace[1],
//                 workspace[2]
//                 );
//         fclose(fp);
                            
//     } // if testing




    // Now apply to the target
    use_flag[0] = true;
    observations_3D[0].station_number = UINT32_MAX;
    observations_3D[0].station_receiver_id = UINT32_MAX;
    observations_3D[0].object_id = UINT32_MAX;
    observations_3D[0].object_number = UINT32_MAX;
    observations_3D[0].station = &telescope;
    LatLon_Cart AltAz = telescope.convert_RADec_to_AltAz(target);
    observations_3D[0].El = AltAz.Lat();
    observations_3D[0].Az = AltAz.Lon();
    observations_3D[0].MJD = MJD_obs;
    observations_3D[0].direction = target;
    Real64 model_STEC = GPS_BAD_DATA_VALUE;
    Real64 model_STEC_err = GPS_BAD_DATA_VALUE;
    Real64 model_SRM = GPS_BAD_DATA_VALUE;
    if((model_ionosphere)) {
        fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_fit calling Integrate_Electron_Faraday\n");
        model_ionosphere->Integrate_Electron_Faraday(
            telescope,
            MJD_obs,
            target,
            &model_STEC,
            &model_STEC_err,
            &model_SRM,
            telescope.Elevation());
        if(!finite(model_STEC)) {
            fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
            exit(1);
        }
        observations_3D[0].model_STEC = model_STEC;
        observations_3D[0].sigma_model_STEC = 0.0;
    }
    else {
        observations_3D[0].model_STEC = 0.0;
        observations_3D[0].sigma_model_STEC = -1.0;
    }
    if((model_ionosphere)) {
        delete model_ionosphere;
    }

    switch(fit_type) {
        case multiple_3D_mult :
            apply_multilayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        case multiple_3D_many :
            apply_manylayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        case multiple_3D_sphe :
        case multiple_3D_sphg :
            apply_manylayer_spherical_h_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Station_Bias)) ? NUM_RECEIVER*num_receiver_types : 0,
                ((Fit_Object_Bias)) ? num_sats:0,  
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    
//#ifdef DEBUG
//      fprintf(stderr, "get_multiple_receiver_3D_MIM_fit: Got fit value %12.3E %10.1E     %9.3f  %9.3f at %12.5f\n", observations_3D[0].model_STEC, observations_3D[0].sigma_model_STEC, observations_3D[0].El*M_RAD2DEG, observations_3D[0].Az*M_RAD2DEG, MJD_obs);
//#endif

    if(observations_3D[0].sigma_model_STEC < 0.0)
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    
    Real64 error_estimate = std_dev;

    Real64 sigma_SRM = GPS_BAD_DATA_VALUE;
    if(model_SRM != GPS_BAD_DATA_VALUE) {
        sigma_SRM = error_estimate * model_SRM / model_STEC;
        model_SRM *= workspace[NUM_POLY-1];
    }
    return GPS_collection_return(observations_3D[0].model_STEC,
                                 error_estimate,
                                 model_SRM,
                                 sigma_SRM);
}











//_TITLE  get_multiple_receiver_3D_MIM_track_fit
GPS_collection_return GPS_collection::get_multiple_receiver_3D_MIM_track_fit(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs,      // I  The time of the observation, as an MJD.
    const GPS_Fit_Type_Enum fit_type,
                               // I  Do a multi or a many-layer fit
    const bool use_IRI_data,   // I  Use or not an updated IRI ionosphere for the
                               //    3-D height scaling information
    const bool Force_Zero_Mean_Bias,// I  Should the bias level be forced to 0?
    const bool Hold_Valid_Station_Bias,//I  Hold or not receivers with valid bias
    const bool Fit_Track_Bias,// I  Fit or not the track(satellite) biases
    const GPS_receiver* const Central_Receiver_Only
                               // I  If this is not NULL, then allow this
                               //    and only this receiver to have its bias
                               //    level vary and be fit.
        )
// GPS_collection_return GPS_collection::get_multiple_receiver_3D_MIM_track_fit
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here
    
//_DESC  full description of program
//	This function uses nearby (hopefully many more than one) GPS receivers
//	to calculate a 3-D MIM fit to the ionosphere.  It then calculates
//	the model prediction for the telescope line of sight to the target.

//	This 3D implementation of the MIM algorithm is specialized for the
//	case of using separate bias levels for EACH INDIVIDUAL SATELLITE TRACK.
//	It appears that each track of a different satellite at each receiver
//	may need a separate bias level.  This function is designed to
//	deal with finding those bias levels.

//	Beacuse this vastly increases the number of free parameters in the
//	fit, this function should only be used with a full global dataset.  

//	The 3-D MIM model currently has 2 options.  The multilayer model
//	assumes that the latitude and longitude variation across the ionosphere
//	is relatively the same at all heights above the Earth.  Multiple
//	layers are used to model the ionosphere with multiple pierce points
//	through a simple 2-D structure.

//	The manylayer model, in contrast, assumes that each height layer
//	can have a different latitude and longitude dependence.  This allows
//	for greater variation, but more parameters are needed to describe the
//	variation at the different heights.

//	At the time of 2007 Jan 04, these two different models seem to produce
//	roughly equivalent results for about the same number of parameters.
//	It is not clear that either way is better than the other at the moment.


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Apr 19  James M Anderson  --JIVE  start from
//                                       get_multiple_receiver_3D_MIM_fit

//_END

{
    // Get the list of nearby receivers
    GPS_receiver** Receiver_List = NULL;
    Uint32 NUM_RECEIVER = 0;
    get_nearby_receivers(telescope, Receiver_List, NUM_RECEIVER);
    if(NUM_RECEIVER == 0) return GPS_collection_return(GPS_BAD_DATA_VALUE);

    Space_Vector target_pierce =
            telescope.get_pierce_location(target,
                                          selection_criteria.Default_Iono_Height,
                                          radius_Earth);

// This following stuff seems to cause segmentation faults with Scientific Linux.
// Tests with RI_G03/RI_G05 show that it seems to do nothing useful anyway so 
// I'm commenting the whole thing out
/*
    if(use_IRI_data) {
        fprintf(stdout, "In GPS_collection::get_multiple_receiver_3D_MIM_track_fit inside if(use_IRI_data)\n");
        // Ok, update the height profile with the IRI data at the target
        // pierce point
        Station_LatLon t(target_pierce);
        JMA_VEX_AREA::VEX_Time v(JMA_VEX_AREA::MJD_OFFSET, MJD_obs);
        struct tm eval_time = v.C_tm_time();
        fill_IRI_electron_densities(t, eval_time);
    }
*/

    // Ok, find the starting and the ending time
    Uint32 start_index
        = time_arr->get_nearest_index_2(MJD_obs
                                        - selection_criteria.Averaging_Time_Half_Width
                                        * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 end_index
        = time_arr->get_nearest_index(MJD_obs
                                      + selection_criteria.Averaging_Time_Half_Width
                                      * PHYSICAL_CONSTANTS::DAYS_PER_SECOND);
    Uint32 time_increment = 1;
    if(Fit_Track_Bias) {
        if(end_index - start_index > 30) 
            time_increment = (end_index - start_index) / 30;
    }

    // Now figure out how many observations there are.
    Uint32 Num_Obs = 0;
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            // Run through the list of satellites
            Num_Obs += Receiver_List[r]->station_obs.NUM_SATELLITES;
        }
    }
    const Uint32 MAX_NUM_ACTUAL_PARAMETERS =
        selection_criteria.MAX_NUM_PARAMETERS
        +( ((Fit_Track_Bias))? (Satellite_Track_Max_Tracks):0);
    Uint32 Num_Work = 0;
    switch(fit_type) {
        case multiple_3D_mult :
        case multiple_3D_many :
        case multiple_3D_sphe :
            Num_Work =
                (Num_Obs + 1 + GPS_MAX_POSSIBLE_RECEIVER_TYPE*NUM_RECEIVER)
                * (2*MAX_NUM_ACTUAL_PARAMETERS+4)
                + MAX_NUM_ACTUAL_PARAMETERS
                * (MAX_NUM_ACTUAL_PARAMETERS+3)
                + selection_criteria.NUM_HEIGHTS * 5;
            break;
        case multiple_3D_sphg:
            Num_Work = Num_Obs*(2*selection_criteria.MAX_NUM_PARAMETERS+7)
                + MAX_NUM_ACTUAL_PARAMETERS*7
                + selection_criteria.NUM_HEIGHTS * 4;
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type), __FILE__, __LINE__, __func__);
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    reallocate_3D_observations(Num_Obs, Num_Work);

    Ionosphere_Base* const model_ionosphere =
        generate_new_theoretical_ionosphere(selection_criteria.Theo_Model_Type,
                                            GPS_DEFAULT_MODEL_ELECTRON_PRECISION,
                                            GPS_DEFAULT_MODEL_FARADAY_PRECISION);

    // Now add everything to the observation list
    Uint32 obs = 0;
    Uint32 num_tracks = 0; clear_fake_track_num();
    for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
        const Real64 MJD = time_arr->get_time(t);
        for(Uint32 r=0; r < NUM_RECEIVER; r++) {
            // Get the real receiver number
            Uint32 rec_ID_real;
            for(rec_ID_real = 0; rec_ID_real < Num_Valid_Receivers;
                rec_ID_real++) {
                if(rec_list[rec_ID_real] == Receiver_List[r]) break;
            }
            if(rec_ID_real == Num_Receivers) continue;
            // And now on to the satellites
            for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                s++, obs++) {
                use_flag[obs] = true;
                observations_3D[obs].station_number = r;
                observations_3D[obs].station =
                    &(Receiver_List[r]->station_position);
                
                // get this satellite and track info
                Sint16 sat_num;
                Sint16 track_num;
                Real64 STEC;
                Real64 sigma;
                Receiver_List[r]->station_obs.get_data2(t, s, &sat_num,
                                                        &track_num,
                                                        &STEC, &sigma);
                if(sat_num < 0) {
                    use_flag[obs] = false;
                    continue;
                }
                observations_3D[obs].MJD = MJD;
                observations_3D[obs].object_id = sat_num;
                observations_3D[obs].object_number = sat_num;
                observations_3D[obs].STEC = STEC;
                observations_3D[obs].sigma_STEC = sigma;
                if((STEC == GPS_BAD_DATA_VALUE)
                   || (sigma == GPS_BAD_DATA_VALUE)) {
                    use_flag[obs] = false;
                    continue;
                }
                Space_Unit_Vector satellite =
                    (sat_pos->get_sat(t, Uint32(sat_num))
                     - Receiver_List[r]->station_position).make_unit_vector();
                observations_3D[obs].direction = satellite;
                
                Real64 angle = target.Angular_Separation(satellite);
                LatLon_Cart AltAz =
                    Receiver_List[r]->station_position.convert_RADec_to_AltAz(satellite);
                Real64 El   = observations_3D[obs].El = AltAz.Lat();
                /*Real64 Az   =*/ observations_3D[obs].Az = AltAz.Lon();
                
                if(( test_satellite_criteria_fail(target_pierce,
                                                  Receiver_List[r]->station_position,
                                                  satellite,
                                                  angle, El ) )) {
                    use_flag[obs] = false;
                    continue;
                }
                if((model_ionosphere)) {
                    observations_3D[obs].model_STEC =
                        get_receiver_obs_model_STEC(model_ionosphere,
                                                    Receiver_List[r],
                                                    MJD,
                                                    satellite, t, s);
                    if(!finite(observations_3D[obs].model_STEC)) {
                        fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
                        exit(1);
                    }
                    if(observations_3D[obs].model_STEC
                       != GPS_BAD_DATA_VALUE)
                        observations_3D[obs].sigma_model_STEC = 0.0;
                    else observations_3D[obs].sigma_model_STEC = 1.0E24;
                }
                else {
                    observations_3D[obs].model_STEC = 0.0;
                    observations_3D[obs].sigma_model_STEC = -1.0;
                }
                Uint32 fake_track_num = get_fake_track_num(rec_ID_real,
                                                           sat_num,
                                                           track_num,
                                                           num_tracks);
                //printf("track info rec %4u sat %3d track %2d num_tracks %5u fake_track %5u\n", rec_ID_real,sat_num,track_num,num_tracks,fake_track_num);
                if(Central_Receiver_Only == NULL) {
                    Uint32 receiver_type = sat_num/100;
                    Sint32 bias_valid =
                        Receiver_List[r]->station_obs.get_bias_valid(receiver_type);
                    if( ((bias_valid)) && ((Hold_Valid_Station_Bias)) ) {
                        observations_3D[obs].station_receiver_id =UINT32_MAX;
                    }
                    else {
                        observations_3D[obs].station_receiver_id =
                            fake_track_num;
                    }
                }
                else if(Receiver_List[r] == Central_Receiver_Only) {
                    // just allow this one to be fit
                    observations_3D[obs].station_receiver_id =
                        fake_track_num;
                }
                else {
                    // Only doing central receiver, but this one is not it
                    observations_3D[obs].station_receiver_id = UINT32_MAX;
                }
#ifdef DEBUG                        
                fprintf(stderr, "Got %12.5f %6u %3u %3u %4u at %10.3E %9.1E %10.3E    %9.3f  %9.3f\n", time_arr->get_time(t), obs, observations_3D[obs].station_number, observations_3D[obs].object_number, observations_3D[obs].station_receiver_id, observations_3D[obs].STEC, sigma, observations_3D[obs].model_STEC,El*M_RAD2DEG, observations_3D[obs].Az*M_RAD2DEG);
#endif
            } // for s over all satellites
        } // for r over receivers
    } // for t over the number of time indices

    // Ok, how many of these are actually good?
    Uint32 good_obs = 0;
    for(obs=0; obs < Num_Obs; obs++) if(use_flag[obs]) good_obs++;
    if(good_obs == 0) {
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }

    Uint32 Num_Use_Param = (selection_criteria.MAX_NUM_PARAMETERS < good_obs) ?
        selection_criteria.MAX_NUM_PARAMETERS : good_obs;
    if(Num_Use_Param > 3*NUM_RECEIVER) Num_Use_Param = 3*NUM_RECEIVER;
    Uint32 NUM_POLY = Num_Use_Param;
    if((Fit_Track_Bias)) Num_Use_Param += num_tracks;
    fprintf(stderr, "Using %u tracks\n", num_tracks);

    // Now for the actual fit
    Real64 std_dev = 0.0;
    Sint32 return_code = 0;
    // Clear the parameter space
    for(Uint32 p=0; p < Num_Use_Param; p++) workspace[p] = 0.0;
    // If using a model ionosphere, set that parameter to 1
    if((model_ionosphere)) workspace[NUM_POLY-1] = 1.0;
    switch(fit_type) {
        case multiple_3D_mult :
            return_code= fit_multilayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_many :
            return_code= fit_manylayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_sphe :
            return_code= fit_manylayer_spherical_h_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                Num_Obs,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                Force_Zero_Mean_Bias,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        case multiple_3D_sphg :
            return_code= fit_manylayer_spherical_h_model_2_grad(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                MJD_obs,
                Num_Obs,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                workspace + Num_Use_Param,
                observations_3D,
                std_dev,
                workspace
                );
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            if((model_ionosphere)) delete model_ionosphere;
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    
    if((return_code)) {
        // something bad happened
        if((model_ionosphere)) delete model_ionosphere;
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }


    // If we are fitting for track biases, then compute the error
    // for individual tracks
    if(Fit_Track_Bias) {
        const Uint32 NUM_ITEMS = num_tracks;
        Real64* sum_array = workspace + Max_Number_Workspace
            - 3*Satellite_Track_Max_Tracks;
        Real64* sum_sqr_array = sum_array + Satellite_Track_Max_Tracks;
        Real64* count_array = sum_sqr_array + Satellite_Track_Max_Tracks;
        for(Uint32 i=0; i < 3*Satellite_Track_Max_Tracks; i++) {
            sum_array[i] = 0.0;
        }

        obs = 0;
        for(Uint32 t=start_index; t <= end_index; t+=time_increment) {
            for(Uint32 r=0; r < NUM_RECEIVER; r++) {
                for(Uint32 s=0; s < Receiver_List[r]->station_obs.NUM_SATELLITES;
                    s++, obs++) {
                    if(use_flag[obs] == false) continue;
                    Uint32 station_id = observations_3D[obs].station_receiver_id;
                    if(station_id == UINT32_MAX) continue;
                    Real64 Delta_STEC = observations_3D[obs].STEC
                        - observations_3D[obs].model_STEC;
                    sum_array[station_id] += Delta_STEC;
                    sum_sqr_array[station_id] += Delta_STEC * Delta_STEC;
                    count_array[station_id] += 1.0;
                } // for s over all satellites
            } // for r over receivers
        } // for t over the number of time indices
        printf("Information for fit time %.5f\n", MJD_obs);
        for(Uint32 i=0; i < NUM_ITEMS; i++) {
            if(count_array[i] > 4.0) {
                if(sum_sqr_array[i] != 0.0) {
                    Real64 var = ( sum_sqr_array[i]
                                   - sum_array[i] * sum_array[i] / count_array[i])
                        / (count_array[i]-1.0);
                    if(var < 0.0) var = 0.0;
                    Real64 std_dev = sqrt(var);
                    sum_array[i] = std_dev;
                    //printf("For fake_track %4u got residual level %12.2E\n", i, std_dev);
                }
                else sum_array[i] = 0.0;
            }
            else sum_array[i] = 0.0;
        }
    } // if placing station error levels into workspace


    // Now apply to the target
    use_flag[0] = true;
    observations_3D[0].station_number = UINT32_MAX;
    observations_3D[0].station_receiver_id = UINT32_MAX;
    observations_3D[0].object_id = UINT32_MAX;
    observations_3D[0].object_number = UINT32_MAX;
    observations_3D[0].station = &telescope;
    LatLon_Cart AltAz = telescope.convert_RADec_to_AltAz(target);
    observations_3D[0].El = AltAz.Lat();
    observations_3D[0].Az = AltAz.Lon();
    observations_3D[0].MJD = MJD_obs;
    observations_3D[0].direction = target;
    Real64 model_STEC = GPS_BAD_DATA_VALUE;
    Real64 model_STEC_err = GPS_BAD_DATA_VALUE;
    Real64 model_SRM = GPS_BAD_DATA_VALUE;
    if((model_ionosphere)) {
        model_ionosphere->Integrate_Electron_Faraday(
            telescope,
            MJD_obs,
            target,
            &model_STEC,
            &model_STEC_err,
            &model_SRM,
            telescope.Elevation());
        if(!finite(model_STEC)) {
            fprintf(stderr,"Warning: got unreasonable total electron content, setting to zero and zero weight.\n");
            exit(1);
        }
        observations_3D[0].model_STEC = model_STEC;
        observations_3D[0].sigma_model_STEC = 0.0;
    }
    else {
        observations_3D[0].model_STEC = 0.0;
        observations_3D[0].sigma_model_STEC = -1.0;
    }
    if((model_ionosphere)) {
        delete model_ionosphere;
    }

    switch(fit_type) {
        case multiple_3D_mult :
            apply_multilayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        case multiple_3D_many :
            apply_manylayer_linear_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        case multiple_3D_sphe :
        case multiple_3D_sphg :
            apply_manylayer_spherical_h_model_2(
                Num_Use_Param,
                selection_criteria.NUM_HEIGHTS,
                telescope,
                1,
                ((Fit_Track_Bias)) ? num_tracks:0,
                false,
                use_flag,
                workspace,
                std_dev,
                workspace + Num_Use_Param,
                observations_3D
                );
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            return GPS_collection_return(GPS_BAD_DATA_VALUE);
    }
    
//#ifdef DEBUG
//      fprintf(stderr, "get_multiple_receiver_3D_MIM_track_fit: Got fit value %12.3E %10.1E     %9.3f  %9.3f at %12.5f\n", observations_3D[0].model_STEC, observations_3D[0].sigma_model_STEC, observations_3D[0].El*M_RAD2DEG, observations_3D[0].Az*M_RAD2DEG, MJD_obs);
//#endif

    if(observations_3D[0].sigma_model_STEC < 0.0)
        return GPS_collection_return(GPS_BAD_DATA_VALUE);
    
    Real64 error_estimate = std_dev;
    Real64 sigma_SRM = GPS_BAD_DATA_VALUE;
    if(model_SRM != GPS_BAD_DATA_VALUE) {
        sigma_SRM = error_estimate * model_SRM / model_STEC;
        model_SRM *= workspace[NUM_POLY-1];
    }
    return GPS_collection_return(observations_3D[0].model_STEC,
                                 error_estimate,
                                 model_SRM,
                                 sigma_SRM);
}

















//_TITLE  get_GPS_prediction ---get a prediction for a specific direction
GPS_collection_return GPS_collection::get_GPS_prediction(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const Station_Reference& telescope,
                               // I  the telescope position, in Earth-fixed
                               //    coordinates
    const Space_Unit_Vector& target,
                               // I  The observation direction from the
                               //    viewpoint of the telescope, in Earth-fixed
                               //    coordinates.
    const Real64 MJD_obs,      // I  The time of the observation, as an MJD.
    const GPS_Fit_Type_Enum fit_type,
                               // I  Do a multi or a many-layer fit
    const bool use_IRI_data,   // I  Use or not an updated IRI ionosphere for the
                               //    3-D height scaling information
    const bool Fit_Station_Bias,//I  Fit or not the station biases
    const bool Force_Zero_Mean_Bias,// I  Should the bias level be forced to 0?
    const bool Hold_Valid_Station_Bias,//I  Hold or not receivers with valid bias
    const bool Fit_Object_Bias,// I  Fit or not the object(satellite) biases
    const GPS_receiver* const Central_Receiver_Only
                               // I  If this is not NULL, then allow this
                               //    and only this receiver to have its bias
                               //    level vary and be fit.
        )
// GPS_collection_return get_GPS_prediction
//                                O  The STEC value, in m^{-2}
//                                   A value of GPS_BAD_DATA_VALUE indicates
//                                   no valid data available.
//                                   A value of GPS_NOT_INITIALIZED_VALUE
//                                   indicates that the user has not filled in
//                                   all data (times, sat_pos, receiver data).

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	This function is the public interface to the GPS ionospheric prediction
//	functions for single telescope and direction information.  It takes
//	in a telescope position and a target direction as seen by the telescope
//	and a time of observation.  It then uses the fit_type parameter to
//	understand which type of ionospheric processing of the GPS data is
//	to be done, and calls the private functions to handle the
//	requested processing.  It then returns the result, the total electron
//	content along the line of sight to the target.

//	If the result is GPS_BAD_DATA_VALUE, then something has gone
//	wrong, but not necessarily terribly so.  The caller should only
//	assume that no data for the specific time was available (perhaps there
//	was a power glitch at the GPS receiver, or for a brief time no satellites
//	were close to the target object).


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Jan 08  James M Anderson  --JIVE  start
//	2007 Apr 17  JMA  --convert to new STEC/SRM return struct

//_END

{
    fprintf(stderr, "doing fit type %d\n", int(fit_type));
    fprintf(stdout, "In GPS_collection::get_GPS_prediction\n");
    if((time_arr == NULL) || (sat_pos == NULL) || (rec_list == NULL))
        return GPS_collection_return(GPS_NOT_INITIALIZED_VALUE);
    GPS_collection_return return_value(GPS_BAD_DATA_VALUE);
    switch (fit_type) {
        case single_nearest :
            fprintf(stdout, "In GPS_collection::get_GPS_prediction calling get_single_receiver_nearest_STEC\n");
            return_value =
                get_single_receiver_nearest_STEC(telescope, target, MJD_obs);
            break;
        case single_all :
            fprintf(stdout, "In GPS_collection::get_GPS_prediction calling get_single_receiver_average_STEC\n");
            return_value =
                get_single_receiver_average_STEC(telescope, target, MJD_obs);
            break;
        // The next ones have the same function call
        case multiple_2D :
        case multiple_2D_time :
        case multiple_2D_grad :
        case multiple_2D_timg :
            fprintf(stdout, "In GPS_collection::get_GPS_prediction calling get_multiple_receiver_2D_MIM_fit\n");
            return_value =
                get_multiple_receiver_2D_MIM_fit(telescope, target, MJD_obs,
                                                 fit_type,
                                                 Fit_Station_Bias,
                                                 Force_Zero_Mean_Bias,
                                                 Hold_Valid_Station_Bias,
                                                 Fit_Object_Bias,
                                                 Central_Receiver_Only);
            break;
        // The next three have the same function call
        case multiple_3D_mult :
        case multiple_3D_many :
        case multiple_3D_sphe :
        case multiple_3D_sphg :
            switch(selection_criteria.Bias_Fit_Type) {
                case Use_Global_Track_Fitting:
                case Use_Global_Track_Without_Theo:
                    fprintf(stdout, "In GPS_collection::get_GPS_prediction calling get_multiple_receiver_3D_MIM_track_fit\n");
                    return_value = get_multiple_receiver_3D_MIM_track_fit(
                        telescope, target, MJD_obs,
                        fit_type, use_IRI_data,
                        Force_Zero_Mean_Bias,
                        Hold_Valid_Station_Bias,
                        Fit_Object_Bias,
                        Central_Receiver_Only);
                    break;
                default :
                    fprintf(stdout, "In GPS_collection::get_GPS_prediction calling get_multiple_receiver_3D_MIM_fit\n");
                    return_value = get_multiple_receiver_3D_MIM_fit(
                        telescope, target, MJD_obs,
                        fit_type, use_IRI_data,
                        Fit_Station_Bias,
                        Force_Zero_Mean_Bias,
                        Hold_Valid_Station_Bias,
                        Fit_Object_Bias,
                        Central_Receiver_Only);
            }
            break;
        default :
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(fit_type),
                    __FILE__, __LINE__, __func__);
            return_value = GPS_collection_return(GPS_BAD_DATA_VALUE);
    };
#ifdef DEBUG
    if((Total_Bias_Correction_Done_Flag)) {
        printf("Got final value %12.3E \\pm %.1E\n", return_value.STEC, return_value.sigma_STEC); fflush(stdout);
    }
#endif
    return return_value;
}














//_TITLE  test_LA
Real64 GPS_collection::test_LA(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    void
    )
// Real64 test_LA
//                                O  return value
//                                   normally this indicates the abs(max) bias
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 14  James M Anderson  --JIVE  start

//_END

{
    fprintf(stderr, "Warning: doing LA test\n");
    // Ok, first off, save the user's selection criteria, as I need my own.
    GPS_criteria original_user_criteria = selection_criteria;

    // setup new criteria for this receiver
    selection_criteria = GPS_criteria(multiple_3D_mult,
                                      2.0*M_PI,
                                      20.0*M_DEG2RAD, // elevation angle
                                      1000E3, // receiver distance
                                      2000E3, // iono pierce
                                      300E3,  // iono height
                                      15.5,
                                      10, // NUM_PARAMETERS,
                                      1, // num heights
                                      1, // NUM TIME TERMS
                                      selection_criteria.Theo_Model_Type,
                                      selection_criteria.Bias_Fit_Type
                                      );

    Station_Reference station(-2494331.2433646899,
                              -4661671.7566389572,
                              3555637.1036464777);  // somewhere in central LA
    Space_Unit_Vector direction = station.make_unit_vector();


    for(Uint32 t=0; t < time_arr->NUM_TIMES; t++) {
        Real64 MJD = time_arr->get_time(t);

        // do the calculation
        get_GPS_prediction(station,
                           direction,
                           MJD,
                           selection_criteria.Default_Fit_Type,
                           false,
                           false,
                           false,
                           false,
                           false,
                           NULL);
    } // for t over all times

    selection_criteria = original_user_criteria;
    return 0.0;
}
    








//_TITLE  correct_receiver_biases --fix up the receiver bias levels
Real64 GPS_collection::correct_receiver_biases(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    void)
// Real64 GPS_collection::correct_receiver_biases
//                                O  return value
//                                   normally this is 0.0
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which come into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10--100 TECU, so it is extremely
//	important to correct these biases.

//	This routine will try to calculate the bias levels by performing
//	2D fits to the ionosphere, and finding the average bias levels needed
//	to make the data match well.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 02  James M Anderson  --JIVE  start

//_END

{
    fprintf(stderr, "In GPS_collection::correct_receiver_biases\n");

    if((Total_Bias_Correction_Done_Flag)) return 0.0;
    // perform track operations here
    count_total_tracks();
    if((selection_criteria.Bias_Fit_Type == Use_Global_Fitting)
       || (selection_criteria.Bias_Fit_Type == Use_Global_Without_Theoretical)
       || (selection_criteria.Bias_Fit_Type == Use_Global_Track_Fitting)
       || (selection_criteria.Bias_Fit_Type == Use_Global_Track_Without_Theo)) {
        return correct_receiver_biases_global();
    }
    const Uint32 NUM_PARAMETERS =
        (selection_criteria.Max_Rec_Dist_From_Tele > 300E3) ? 20:10;
    const Real64 Averaging_Time = 600.0;

    Real64 retval;
    int step = 0;

//     {
//         for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
//             if(rec_list[receiver] == NULL) continue;
//             char filename[128];
//             sprintf(filename,"dump.%3.3u.pre.out", receiver);
//                 dump_receiver_to_disk(rec_list[receiver],filename);
//         }
//     }
    fprintf(stderr, "Warning: Bias level calibration for GPS data may take many hours\n");

    // Check for at least one primary bias receiver
    bool have_primary = false;
    {
        for(Uint32 receiver = 0; receiver < Num_Valid_Receivers; receiver++) {
            if(rec_list[receiver] == NULL) continue;
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Sint32 bias_valid =
                    rec_list[receiver]->station_obs.get_bias_valid(type);
                if(bias_valid == 1) {
                    have_primary = true;
                    break;
                }
            }
            if((have_primary)) break;
        }
    }

    printf("Starting total bias correction step %d\n", step++);
    if(have_primary) {
        printf("Calling correct_receiver_biases_low\n");
        retval = correct_receiver_biases_low(multiple_2D, NUM_PARAMETERS,
                                             Averaging_Time,
                                             true, false, true, false);
    } else {
        printf("have_primary is False\n");
        printf("Calling correct_receiver_biases_force\n");
        retval = correct_receiver_biases_force(
            multiple_2D_grad,//multiple_2D_time,
            20,
            2*60.0*60.0+0.5,  // 3 hours
            true,
            false);
    }
    if(retval != 0.0) return retval;
//     {
//         for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
//             if(rec_list[receiver] == NULL) continue;
//             char filename[128];
//             sprintf(filename,"dump.%3.3u.sec.out", receiver);
//                 dump_receiver_to_disk(rec_list[receiver],filename);
//         }
//     }
    
    for(Uint32 i=0; i < 9; i++) {
        printf("Starting total bias correction step %d\n", step++);
        retval = correct_receiver_biases_low(multiple_2D, NUM_PARAMETERS,
                                             Averaging_Time,
                                             false, false, true, false);
        if(retval != 0.0) return retval;
    }
    printf("Starting total bias correction step %d\n", step++);
    retval = correct_receiver_biases_low(multiple_2D, NUM_PARAMETERS,
                                         Averaging_Time,
                                         false, false, false, true);
    if(retval != 0.0) return retval;

    
    // Now clear and start over
    for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
        if(rec_list[receiver] == NULL) continue;
        rec_list[receiver]->station_obs.clear_bias();
    }
    if(!have_primary) {
        printf("Starting total bias correction step %d\n", step++);
        retval = correct_receiver_biases_force(
            multiple_2D_time,
            20,
            2*60.0*60.0+0.5,  // 3 hours
            false,
            false);
        if(retval != 0.0) return retval;
    }
    for(Uint32 i=0; i < 9; i++) {
        printf("Starting total bias correction step %d\n", step++);
        retval = correct_receiver_biases_low(multiple_2D, NUM_PARAMETERS,
                                             Averaging_Time,
                                             false, false, true, false);
        if(retval != 0.0) return retval;
    }

    // Check that everyone has at least been corrected
    {
        Real64 return_code = enforce_all_bias_corrected();
        if(return_code != 0) return GPS_NOT_INITIALIZED_VALUE;
    }
    
    printf("Starting total bias correction step %d\n", step++);
    retval = correct_receiver_biases_low(multiple_2D, NUM_PARAMETERS,
                                         Averaging_Time,
                                         false, true, false, false);
    if(retval != 0.0) return retval;
    


    printf("Total bias correction finished\n");fflush(stdout);
    Total_Bias_Correction_Done_Flag = 1;
    return 0.0;
}



//_TITLE  correct_receiver_biases_global --fix up the receiver bias levels
Real64 GPS_collection::correct_receiver_biases_global(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    void)
// Real64 GPS_collection::correct_receiver_biases_global
//                                O  return value
//                                   normally this is 0.0
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which come into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10--100 TECU, so it is extremely
//	important to correct these biases.
    
//	This routine assumes that you have stations positioned around the Earth
//	(typically achieved through the GPS_stations.add_global_stations_to_list
//	function in the Python code).  Using many stations around the Earth,
//	a good bias level system can be determined for all stations/satellites at
//	once.  This requires a large number of stations, and a huge number of
//	datapoints and parameters, so gradient minimization least squares is
//	required.  But, everything should be able to be fit in one go.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 02  James M Anderson  --JIVE  start

//_END

{
    printf("InGPS_collection::correct_receiver_biases_global with theo %d bias %d\n",int(selection_criteria.Theo_Model_Type), int(selection_criteria.Bias_Fit_Type));
    fprintf(stderr,"InGPS_collection::correct_receiver_biases_global with theo %d bias %d\n",int(selection_criteria.Theo_Model_Type), int(selection_criteria.Bias_Fit_Type));
    if((Total_Bias_Correction_Done_Flag)) return 0.0;
    const Uint32 NUM_PARAMETERS = 75;
    const Real64 Averaging_Time = 3600.0*2;

    Real64 retval;
    int step = 0;

    fprintf(stderr, "Warning: Global bias level calibration for GPS data may take many hours\n");


    fprintf(stderr,"Starting total bias correction step %d\n", step++);
    // This step will compute biases and also error estimates
    switch (selection_criteria.Bias_Fit_Type) {
        case Use_Global_Fitting:
        case Use_Global_Without_Theoretical:
            retval = correct_receiver_biases_global_force(
                multiple_3D_sphg,
                NUM_PARAMETERS,
                Averaging_Time,
                true
                );
            break;
        case Use_Global_Track_Fitting:
        case Use_Global_Track_Without_Theo:
            retval = correct_receiver_biases_track_force(
                multiple_3D_sphg,
                NUM_PARAMETERS,
                Averaging_Time,
                true
                );
            break;
        default:
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(selection_criteria.Bias_Fit_Type), __FILE__, __LINE__, __func__);
            return GPS_NOT_INITIALIZED_VALUE;
    }
    if(retval != 0.0) return retval;

    // Now that the error estimates have been updated, recompute
    // the bias estimates
    printf("Starting total bias correction step %d\n", step++);
    switch (selection_criteria.Bias_Fit_Type) {
        case Use_Global_Fitting:
        case Use_Global_Without_Theoretical:
            retval = correct_receiver_biases_global_force(
                multiple_3D_sphg,
                NUM_PARAMETERS,
                Averaging_Time,
                false
                );
            break;
        case Use_Global_Track_Fitting:
        case Use_Global_Track_Without_Theo:
            retval = correct_receiver_biases_track_force(
                multiple_3D_sphg,
                NUM_PARAMETERS,
                Averaging_Time,
                false
                );
            break;
        default:
            fprintf(stderr, "Error: programmer mistake type==%d in %s:%d:%s\n",
                    int(selection_criteria.Bias_Fit_Type), __FILE__, __LINE__, __func__);
            return GPS_NOT_INITIALIZED_VALUE;
    }
    if(retval != 0.0) return retval;
    
//     {
//         for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
//             if(rec_list[receiver] == NULL) continue;
//             char filename[128];
//             sprintf(filename,"dump.%3.3u.sec.out", receiver);
//                 dump_receiver_to_disk(rec_list[receiver],filename);
//         }
//     }


    printf("Total bias correction finished\n");fflush(stdout);
    Total_Bias_Correction_Done_Flag = 1;
    return 0.0;
}








//_TITLE  correct_receiver_biases_low --fix up the receiver bias levels low level
Real64 GPS_collection::correct_receiver_biases_low(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const GPS_Fit_Type_Enum Fit_Type,//I The type of fit to perform
    const Uint32 NUM_PARAMETERS_IN,//I  number of polynomial parameters to allow
    const Real64 Averaging_Time,//I  Averaging time of the data in s
    const bool Apply_RMS_To_BIAS,//I Apply the RMS to correct bias level?
    const bool Force_Zero_Mean_Bias,// I  Should the bias level be forced to 0?
    const bool Hold_Valid_Station_Bias,//I  Hold or not receivers with valid bias
    const bool Release_Only_Central_Receiver
                                // I  Should this function only allow the central
                                //    receiver to have a varying bias level,
                                //    or shoud all receivers have variable bias
                                //    levels?
    )
// Real64 GPS_collection::correct_receiver_biases
//                                O  return value
//                                   normally this indicates the abs(max) bias
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which comes into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10 TECU, so it is extremely
//	important to correct these biases.

//	This routine will try to calculate the bias levels by performing
//	2D fits to the ionosphere, and finding the average bias levels needed
//	to make the data match well.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Feb 23  James M Anderson  --JIVE  start

//_END

{
    fprintf(stderr,"In GPS_collection::correct_receiver_biases_low\n");
//     const Uint32 NUM_RECEIVER_LOW = 15;
//     const Uint32 NUM_RECEIVER_HIGH = 20;
    const Uint32 NUM_RECEIVER_LOW = 10;
    const Uint32 NUM_RECEIVER_HIGH = 15;
    if((time_arr == NULL) || (sat_pos == NULL) || (rec_list == NULL))
        return GPS_NOT_INITIALIZED_VALUE;

    //FILE* fp = fopen("bias.dat", "a");
     
    // Ok, first off, save the user's selection criteria, as I need my own.
    GPS_criteria original_user_criteria = selection_criteria;

    // Now, I need some workspace
    printf("number of valid receivers %d\n",Num_Valid_Receivers);
    std::vector<Real64> station_sum(Num_Receivers
                                    *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_sumsqr(Num_Receivers
                                       *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_error(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Uint32> station_count(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);


    Real64 start_MJD = time_arr->get_time(0)
        + PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;
    Real64 end_MJD = time_arr->get_time(time_arr->NUM_TIMES-1);

    // loop over the receivers
    for(Uint32 receiver = 0; receiver < Num_Valid_Receivers; receiver++) {

        // clear the summation areas for this receiver
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            station_sum[Num_Receivers*type+receiver]
                = station_sumsqr[Num_Receivers*type+receiver]
                = station_error[Num_Receivers*type+receiver] = 0.0;
            station_count[Num_Receivers*type+receiver] = 0;
        }
        if(rec_list[receiver] == NULL) continue;

        fprintf(stderr, "Working on station %u/%u\n", receiver, Num_Valid_Receivers);
        if((Hold_Valid_Station_Bias)) {
            bool all_valid = true;
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Sint32 bias_valid =
                    rec_list[receiver]->station_obs.get_bias_valid(type);
                fprintf(stderr, "For main receiver, type %u has bias_valid %d\n",type,int(bias_valid));
                if(bias_valid == 0) {
                    all_valid = false;
                    break;
                }
            }
            fprintf(stderr, "Receiver %d has bias_valid code %d\n", receiver,rec_list[receiver]->station_obs.get_bias_valid(0));
            if((all_valid)) {
                // this receiver has valid biases for all receiver types, but we
                // are holding these constant, so skip
                fprintf(stderr, "all bias levels valid, skipping\n");
                continue;
            }
        }
        if((Release_Only_Central_Receiver)) {
            // only primary bias calibrators should be allowed to vary
            bool any_valid = false;
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Sint32 bias_valid =
                    rec_list[receiver]->station_obs.get_bias_valid(type);
                if(bias_valid == 1) {
                    any_valid = true;
                    break;
                }
            }
            if(!any_valid) {
                // this receiver has no primary bias source, so continue
                continue;
            }
        }

        // setup new criteria for this receiver
        Ionosphere_Theoretical_Model_Enum theo_use =
            selection_criteria.Theo_Model_Type;
        if((selection_criteria.Bias_Fit_Type == Use_Main_Without_Theoretical)
           ||(selection_criteria.Bias_Fit_Type == Use_Global_Without_Theoretical)
           ||(selection_criteria.Bias_Fit_Type == Use_Global_Track_Without_Theo)
           ) {
            theo_use = None;
        }
        selection_criteria = GPS_criteria(Fit_Type,
                                          2.0*M_PI,
                                          15.0*M_DEG2RAD, // elevation angle
                                          1000E3, // receiver distance
                                          2000E3, // iono pierce
                                          300E3,  // iono height
                                          Averaging_Time * 0.5,
                                          NUM_PARAMETERS_IN,
                                          4, // num heights
                                          1, // NUM_TIME_TERMS
                                          theo_use,
                                          selection_criteria.Bias_Fit_Type
                                          );
        Station_Reference station = rec_list[receiver]->station_position;
        Space_Unit_Vector direction = station.make_unit_vector();


        // Get the list of nearby receivers
        GPS_receiver** Receiver_List = NULL;
        Uint32 NUM_RECEIVER = UINT32_MAX;
        for(Uint32 loop = 0; loop < 100; loop++) {
            get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
            //printf("Got %u receivers close to rec %u\n", NUM_RECEIVER, receiver);
            if(NUM_RECEIVER > NUM_RECEIVER_HIGH) {
                selection_criteria.Max_Rec_Dist_From_Tele *= 0.95;
            }
            else {
                if((loop > 0) && (NUM_RECEIVER < NUM_RECEIVER_LOW)) {
                    selection_criteria.Max_Rec_Dist_From_Tele *= 20.0/19.0;
                    get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
                }
                break;
            }
        }
        Uint32 NUM_PARAMETERS = NUM_PARAMETERS_IN;
        if(NUM_PARAMETERS > 3*NUM_RECEIVER) NUM_PARAMETERS = 3*NUM_RECEIVER;
        selection_criteria.MAX_NUM_PARAMETERS = NUM_PARAMETERS;

        bool valid_bias_receiver = false;
        for(Uint32 r_b = 0; r_b < NUM_RECEIVER; r_b++) {
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Sint32 bias_valid =
                    Receiver_List[r_b]->station_obs.get_bias_valid(type);
                fprintf(stderr, "For receiver %u type %u bias_valid %d\n",r_b,type,bias_valid);
                if(bias_valid > 0) {
                    valid_bias_receiver = true;
                }
            }
        }
        if((!Apply_RMS_To_BIAS) && (Hold_Valid_Station_Bias)
           && (!valid_bias_receiver)) {
            // we are trying to calibrate using existing bias levels, but
            // there are no pre-existing biases to use.  Skip.
            fprintf(stderr, "no valid bias receivers in list, skipping\n");
            continue;
        }
        if(NUM_RECEIVER <= 1) {
            continue;
        }
        //printf("Got final %u receivers close to rec %u\n", NUM_RECEIVER, receiver);
        // Find the position of the receiver in the temporary receiver list
        Uint32 receiver_bias_pos = 0;
        for(; receiver_bias_pos < NUM_RECEIVER; receiver_bias_pos++)
            if(Receiver_List[receiver_bias_pos] == rec_list[receiver]) break;
        assert(receiver_bias_pos < NUM_RECEIVER);
        printf("Found receiver %u at position %u\n", receiver, receiver_bias_pos);

        for(Real64 MJD = start_MJD; MJD < end_MJD;
            MJD += Averaging_Time * PHYSICAL_CONSTANTS::DAYS_PER_SECOND
            + 0.02) {

            
            // do the calculation
            const GPS_receiver* const  central =
                ((Release_Only_Central_Receiver)) ? rec_list[receiver] : NULL;
            GPS_collection_return ionosphere =
                get_GPS_prediction(station,
                                   direction,
                                   MJD,
                                   selection_criteria.Default_Fit_Type,
                                   false, true,
                                   Force_Zero_Mean_Bias,
                                   Hold_Valid_Station_Bias,
                                   false,
                                   central);
            if((ionosphere.STEC == GPS_BAD_DATA_VALUE)
               || (ionosphere.STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
            // Now, this has filled in some things into our class member objects
            // The bias for the receiver is
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Real64 bias = workspace[NUM_PARAMETERS + NUM_RECEIVER*type
                                        +receiver_bias_pos];
                // Now where is the station error located?
                Real64 sta_err = workspace[Max_Number_Workspace
                                           -3*NUM_RECEIVER
                                           *GPS_MAX_POSSIBLE_RECEIVER_TYPE
                                           + NUM_RECEIVER*type
                                           + receiver_bias_pos];
                if((bias == 0.0) && (sta_err == 0.0)) continue;
                //printf("At MJD %10.4f got bias %.2E\n", MJD, bias);
                //fprintf(fp, "%u %u %.5f %.3E\n", receiver, type, MJD, bias);
                station_sum[Num_Receivers*type+receiver] += bias;
                station_sumsqr[Num_Receivers*type+receiver] += bias*bias;
                if(ionosphere.sigma_STEC > 0.0)
                    station_error[Num_Receivers*type+receiver] +=
                        ionosphere.sigma_STEC;
                station_count[Num_Receivers*type+receiver]++;
            }
        } // for MJD over time
        // Get average bias level
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Real64 average = 0.0;
            Real64 std_dev = 0.0;
            Real64 mean_error = 0.0;
            Uint32 count = station_count[Num_Receivers*type+receiver];
            if(count > 0) {
                average = station_sum[Num_Receivers*type+receiver] / count;
                if(count > 1) {
                    std_dev = (station_sumsqr[Num_Receivers*type+receiver]
                               - station_sum[Num_Receivers*type+receiver]
                               *station_sum[Num_Receivers*type+receiver] / count)
                        / (count-1);
                    // This is the RMS level, find the uncertainty in the average
                    std_dev /= Real64(count);
                    std_dev = (std_dev > 0.0) ? sqrt(std_dev) : 0.0;
                }
                mean_error = station_error[Num_Receivers*type+receiver] / count;
            }
            station_sum[Num_Receivers*type+receiver] = average;
            station_sumsqr[Num_Receivers*type+receiver] = std_dev;
            if( ((Hold_Valid_Station_Bias)) && (valid_bias_receiver) ) {
                station_error[Num_Receivers*type+receiver] = -mean_error;
            } else {
                station_error[Num_Receivers*type+receiver] = mean_error;
            }
            printf("Got station %u average %.2E %.2E from %u times\n", Num_Receivers*type+receiver, average, std_dev, count);
        }
    }


    // Now then, the standard deviation in this biases tend to arrise because
    // of noise in just a few receivers which causes the bias levels of all
    // receivers to bounce up and down.  Assume that the RMS level in the
    // best receiver is an indication of this floating, and subtract it off.
    // At this time, also correct the bias level of the data.
    {
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Real64 var = 0.0;
            if((Apply_RMS_To_BIAS)) {
                Real64 RMS_min = 1E300;
                for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
                    if(rec_list[receiver] == NULL) continue;
                    if(station_error[Num_Receivers*type+receiver]==0.0) continue;
                    if(fabs(station_error[Num_Receivers*type+receiver])< RMS_min)
                        RMS_min=fabs(station_error[Num_Receivers*type+receiver]);
                }
                var = RMS_min * RMS_min;
            }
            for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
                if(rec_list[receiver] == NULL) continue;
                //fprintf(stderr, "Receiver %d has err %E\n", receiver, station_error[Num_Receivers*type+receiver]);
                bool bump_valid =
                    (station_error[Num_Receivers*type+receiver] < 0.0) ? true:false;
                if((Apply_RMS_To_BIAS)) {
                    bump_valid = false;
                    Real64 sub = station_error[Num_Receivers*type+receiver]
                        * station_error[Num_Receivers*type+receiver] - var;
                    sub = (sub > 0.0) ? sqrt(sub):0.0;
                    station_error[Num_Receivers*type+receiver] = sub;
                }
                Real64 bias_level = station_sum[Num_Receivers*type+receiver];
                Real64 err = ((Apply_RMS_To_BIAS)) ?
                    station_error[Num_Receivers*type+receiver]
                    +station_sumsqr[Num_Receivers*type+receiver]
                    :0.0;
                //fprintf(stderr, "For receiver %u, have %11.3E %9.1E %1u %1d\n",receiver, bias_level, err, type, int(bump_valid));
                rec_list[receiver]->
                    station_obs.adjust_bias(bias_level,
                                            err,
                                            type,
                                            bump_valid);
            }
        }
    }

    //fclose(fp);
    

    selection_criteria = original_user_criteria;
    return 0.0;
}















//_TITLE  correct_receiver_biases_force --force fixing the receiver bias levels 
Real64 GPS_collection::correct_receiver_biases_force(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const GPS_Fit_Type_Enum Fit_Type,//I The type of fit to perform
    const Uint32 NUM_PARAMETERS_IN,//I  number of polynomial parameters to allow
    const Real64 Averaging_Time,//I  Averaging time of the data in s
    const bool Apply_RMS_To_BIAS,//I Apply the RMS to correct bias level?
    const bool Force_Zero_Mean_Bias// I  Should the bias level be forced to 0?
    )
// Real64 GPS_collection::correct_receiver_biases_force
//                                O  return value
//                                   normally this indicates the abs(max) bias
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which comes into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10 TECU, so it is extremely
//	important to correct these biases.

//	This routine will try to calculate the bias levels by performing
//	2D fits to the ionosphere, and finding the average bias levels needed
//	to make the data match well.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Feb 23  James M Anderson  --JIVE  start
//	2007 Mar 19  JMA  --adapt for forcing bias level corrections

//_END

{
    printf("In correct_receiver_biases_force\n");
    const Uint32 NUM_RECEIVER_LOW = 20;
    const Uint32 NUM_RECEIVER_HIGH = 25;
    if((time_arr == NULL) || (sat_pos == NULL) || (rec_list == NULL))
        return GPS_NOT_INITIALIZED_VALUE;

    // Ok, first off, save the user's selection criteria, as I need my own.
    GPS_criteria original_user_criteria = selection_criteria;

    // Now, I need some workspace
    std::vector<Real64> station_sum(Num_Receivers
                                    *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_sumsqr(Num_Receivers
                                       *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_error(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Uint32> station_count(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);


    Real64 start_MJD = time_arr->get_time(0)
        + PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;
    Real64 end_MJD = time_arr->get_time(time_arr->NUM_TIMES-1)
        - PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;
    Uint32 Expected_Count_Rate =
        Uint32((time_arr->get_time(time_arr->NUM_TIMES-1)
                - time_arr->get_time(0))
               / (PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time
                  * 0.25+0.02));
    if(Expected_Count_Rate < 2) Expected_Count_Rate = 2;

    // loop over the receivers
    for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
        fprintf(stderr, "Working on station %u/%u\n", receiver, Num_Receivers);

        if(rec_list[receiver] == NULL) continue;

        {
            bool any_valid = false;
            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Sint32 bias_valid =
                    rec_list[receiver]->station_obs.get_bias_valid(type);
                if(bias_valid > 0) {
                    any_valid = true;
                    break;
                }
            }
            if((any_valid)) {
                // this receiver has a valid bias for some type, so skip
                fprintf(stderr, "some bias levels valid, skipping\n");
                continue;
            }
        }

        if(station_count[receiver] >= 3*Expected_Count_Rate) {
            fprintf(stderr, "already checked this receiver many times, skipping\n");
            continue;
        }

        // setup new criteria for this receiver
        Ionosphere_Theoretical_Model_Enum theo_use =
            selection_criteria.Theo_Model_Type;
        if((selection_criteria.Bias_Fit_Type == Use_Main_Without_Theoretical)
           ||(selection_criteria.Bias_Fit_Type == Use_Global_Without_Theoretical)
           ||(selection_criteria.Bias_Fit_Type == Use_Global_Track_Without_Theo)
           ) {
            theo_use = None;
        }
        selection_criteria = GPS_criteria(Fit_Type,
                                          2.0*M_PI,
                                          15.0*M_DEG2RAD, // elevation angle
                                          1000E3, // receiver distance
                                          2000E3, // iono pierce
                                          300E3,  // iono height
                                          Averaging_Time * 0.5,
                                          NUM_PARAMETERS_IN,
                                          4, // num heights
                                          2, // NUM_TIME_TERMS
                                          theo_use,
                                          selection_criteria.Bias_Fit_Type
                                          );
        Station_Reference station = rec_list[receiver]->station_position;
        Space_Unit_Vector direction = station.make_unit_vector();


        // Get the list of nearby receivers
        GPS_receiver** Receiver_List = NULL;
        Uint32 NUM_RECEIVER = UINT32_MAX;
        for(Uint32 loop = 0; loop < 100; loop++) {
            get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
            //printf("Got %u receivers close to rec %u\n", NUM_RECEIVER, receiver);
            if(NUM_RECEIVER > NUM_RECEIVER_HIGH) {
                selection_criteria.Max_Rec_Dist_From_Tele *= 0.95;
            }
            else {
                if((loop > 0) && (NUM_RECEIVER < NUM_RECEIVER_LOW)) {
                    selection_criteria.Max_Rec_Dist_From_Tele *= 20.0/19.0;
                    get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
                }
                break;
            }
        }
        if(NUM_RECEIVER == 0) continue;

        Uint32 NUM_PARAMETERS = NUM_PARAMETERS_IN;
        if(NUM_PARAMETERS > 3*NUM_RECEIVER) NUM_PARAMETERS = 3*NUM_RECEIVER;
        selection_criteria.MAX_NUM_PARAMETERS = NUM_PARAMETERS;

        for(Real64 MJD = start_MJD; MJD < end_MJD;
            MJD += Averaging_Time * PHYSICAL_CONSTANTS::DAYS_PER_SECOND * 0.25
            + 0.02) {

            
            // do the calculation
            GPS_collection_return ionosphere =
                get_GPS_prediction(station,
                                   direction,
                                   MJD,
                                   selection_criteria.Default_Fit_Type,
                                   false, true,
                                   Force_Zero_Mean_Bias,
                                   true,
                                   false,
                                   NULL);
            if((ionosphere.STEC == GPS_BAD_DATA_VALUE)
               || (ionosphere.STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
            // Find the position of the receivers in the temporary receiver list
            for(Uint32 rec_0 = 0; rec_0 < Num_Receivers; rec_0++) {
                if(rec_list[rec_0] == NULL) continue;
                Uint32 receiver_bias_pos = 0;
                for(; receiver_bias_pos < NUM_RECEIVER; receiver_bias_pos++) {
                    if(Receiver_List[receiver_bias_pos] == rec_list[rec_0])
                        break;
                }
                if(receiver_bias_pos == NUM_RECEIVER) continue;

                for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                    Real64 bias = workspace[NUM_PARAMETERS + NUM_RECEIVER*type
                                            +receiver_bias_pos];
                    // Now where is the station error located?
                    Real64 sta_err = workspace[Max_Number_Workspace
                                               -3*NUM_RECEIVER
                                               *GPS_MAX_POSSIBLE_RECEIVER_TYPE
                                               + NUM_RECEIVER*type
                                               + receiver_bias_pos];
                    if((bias == 0.0) && (sta_err == 0.0)) continue;
                    //printf("At MJD %10.4f got bias %.2E\n", MJD, bias);
                    //printf("%u %u %.5f %.3E\n", receiver, type, MJD, bias);
                    station_sum[Num_Receivers*type+rec_0] += bias;
                    station_sumsqr[Num_Receivers*type+rec_0] += bias*bias;
                    if(ionosphere.sigma_STEC > 0.0)
                        station_error[Num_Receivers*type+rec_0] +=
                            ionosphere.sigma_STEC;
                    station_count[Num_Receivers*type+rec_0] += 1;
                }
            }
        } // for MJD over time
    }



    // loop over the receivers
    for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
        if(rec_list[receiver] == NULL) continue;
        // Get average bias level
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Uint32 count = station_count[Num_Receivers*type+receiver];
            Real64 average = 0.0;
            Real64 std_dev = 0.0;
            Real64 mean_error = 0.0;
            if(count > 0) {
                average = station_sum[Num_Receivers*type+receiver] / count;
                if(count > 1) {
                    std_dev = (station_sumsqr[Num_Receivers*type+receiver]
                               - station_sum[Num_Receivers*type+receiver]
                               *station_sum[Num_Receivers*type+receiver] / count)
                        / (count-1);
                    // This is the RMS level, find the uncertainty in the average
                    std_dev /= Real64(count);
                    std_dev = (std_dev > 0.0) ? sqrt(std_dev) : 0.0;
                }
                mean_error = station_error[Num_Receivers*type+receiver] / count;
            }
            station_sum[Num_Receivers*type+receiver] = average;
            station_sumsqr[Num_Receivers*type+receiver] = std_dev;
            station_error[Num_Receivers*type+receiver] = mean_error;
            printf("Got station %u average %.2E %.2E from %u times\n", Num_Receivers*type+receiver, average, std_dev, count);
        }
    }
    

    // Now then, the standard deviation in this biases tend to arrise because
    // of noise in just a few receivers which causes the bias levels of all
    // receivers to bounce up and down.  Assume that the RMS level in the
    // best receiver is an indication of this floating, and subtract it off.
    // At this time, also correct the bias level of the data.
    {
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Real64 var = 0.0;
            if((Apply_RMS_To_BIAS)) {
                Real64 RMS_min = 1E300;
                for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
                    if(rec_list[receiver] == NULL) continue;
                    if(station_error[Num_Receivers*type+receiver]==0.0) continue;
                    if(fabs(station_error[Num_Receivers*type+receiver])< RMS_min)
                        RMS_min=fabs(station_error[Num_Receivers*type+receiver]);
                }
                var = RMS_min * RMS_min;
            }
            for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
                if(rec_list[receiver] == NULL) continue;
                {
                    bool any_valid = false;
                    for(Uint32 type2 = 0; type2 < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type2++) {
                        Sint32 bias_valid =
                            rec_list[receiver]->station_obs.get_bias_valid(type2);
                        if(bias_valid > 0) {
                            any_valid = true;
                            break;
                        }
                    }
                    if((any_valid)) {
                        // this receiver has a valid bias for some type, so skip
                        continue;
                    }
                }
                if((station_error[Num_Receivers*type+receiver] == 0.0)
                   &&(station_sum[Num_Receivers*type+receiver] == 0.0)) continue;
                if((Apply_RMS_To_BIAS)) {
                    Real64 sub = station_error[Num_Receivers*type+receiver]
                        * station_error[Num_Receivers*type+receiver] - var;
                    sub = (sub > 0.0) ? sqrt(sub):0.0;
                    station_error[Num_Receivers*type+receiver] = sub;
                }
                Real64 bias_level = station_sum[Num_Receivers*type+receiver];
                Real64 err = ((Apply_RMS_To_BIAS)) ?
                    station_error[Num_Receivers*type+receiver]
                    +station_sumsqr[Num_Receivers*type+receiver]
                    :0.0;
                //fprintf(stderr, "For receiver %u, have %11.3E %9.1E %1u\n",receiver, bias_level, err, type);
                rec_list[receiver]->
                    station_obs.adjust_bias(bias_level,
                                            err,
                                            type,
                                            true);
            }
        }
    }

    selection_criteria = original_user_criteria;
    return 0.0;
}
    



//_TITLE  correct_receiver_biases_global_force --force global receiver biases
Real64 GPS_collection::correct_receiver_biases_global_force(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const GPS_Fit_Type_Enum Fit_Type,//I The type of fit to perform
    Uint32 NUM_PARAMETERS,      //I  number of polynomial parameters to allow
    const Real64 Averaging_Time,//I  Averaging time of the data in s
    const bool Apply_RMS_To_BIAS//I Apply the RMS to correct bias level?
    )
// Real64 GPS_collection::correct_receiver_biases_global_force
//                                O  return value
//                                   normally this indicates the abs(max) bias
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which comes into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10 TECU, so it is extremely
//	important to correct these biases.

//	This routine calculates biases for all receivers at once, using a
//	global method (hopefully something like spherical harmonics).  It
//	only has to go through the receiver list once, but the number of
//	datapoints and parameters is huge, so gradient minimization least
//	squares is hopefully used.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Feb 23  James M Anderson  --JIVE  start
//	2007 Mar 19  JMA  --adapt for forcing bias level corrections
//	2007 Mar 27  JMA  --adapt to a global solution

//_END

{
    if((time_arr == NULL) || (sat_pos == NULL) || (rec_list == NULL))
        return GPS_NOT_INITIALIZED_VALUE;

    // Ok, first off, save the user's selection criteria, as I need my own.
    GPS_criteria original_user_criteria = selection_criteria;
    
    // Now, I need some workspace
    std::vector<Real64> station_sum(Num_Receivers
                                    *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_sumsqr(Num_Receivers
                                       *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> station_error(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Uint32> station_count(Num_Receivers
                                      *GPS_MAX_POSSIBLE_RECEIVER_TYPE);
    std::vector<Real64> satellite_sum(GPS_MAX_POSSIBLE_SATELLITES);
    std::vector<Real64> satellite_count(GPS_MAX_POSSIBLE_SATELLITES);


    Real64 start_MJD = time_arr->get_time(0)
        + PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;
    Real64 end_MJD = time_arr->get_time(time_arr->NUM_TIMES-1)
        - PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;

    // setup new criteria for this receiver
    Ionosphere_Theoretical_Model_Enum theo_use =
        selection_criteria.Theo_Model_Type;
    if((selection_criteria.Bias_Fit_Type == Use_Main_Without_Theoretical)
       ||(selection_criteria.Bias_Fit_Type == Use_Global_Without_Theoretical)
       ||(selection_criteria.Bias_Fit_Type == Use_Global_Track_Without_Theo)) {
        theo_use = None;
    }
    selection_criteria = GPS_criteria(Fit_Type,
                                      2.0*M_PI,
                                      20.0*M_DEG2RAD, // elevation angle
                                      2E7, // receiver distance
                                      2E7, // iono pierce
                                      300E3,  // iono height
                                      Averaging_Time * 0.5,
                                      NUM_PARAMETERS,
                                      2, // num heights
                                      2, // NUM_TIME_TERMS
                                      theo_use,
                                      selection_criteria.Bias_Fit_Type
                                      );
    // Set up the reference location as 0 lat, 0 lon
    Station_Reference station(0.0,0.0);
    Space_Unit_Vector direction = station.make_unit_vector();

    // Get the list of nearby receivers, to order the list as the
    // fitting routine does.
    GPS_receiver** Receiver_List = NULL;
    Uint32 NUM_RECEIVER = UINT32_MAX;
    get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
    if(NUM_RECEIVER == 0) {
        // Hey, this should never happen
        selection_criteria = original_user_criteria;
        return GPS_NOT_INITIALIZED_VALUE;
    }
    if(NUM_PARAMETERS > 3*NUM_RECEIVER) NUM_PARAMETERS = 3*NUM_RECEIVER;
    selection_criteria.MAX_NUM_PARAMETERS = NUM_PARAMETERS;


    for(Real64 MJD = start_MJD; MJD < end_MJD;
        MJD += (Averaging_Time * 0.25 + 30.0)
            * PHYSICAL_CONSTANTS::DAYS_PER_SECOND) {
            // do the calculation
        GPS_collection_return ionosphere =
            get_GPS_prediction(station,
                               direction,
                               MJD,
                               selection_criteria.Default_Fit_Type,
                               false, true,
                               false,
                               false,
                               true, // fit object bias
                               NULL);
        if((ionosphere.STEC == GPS_BAD_DATA_VALUE)
           || (ionosphere.STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
        // Find the position of the receivers in the temporary receiver list
        for(Uint32 rec_0 = 0; rec_0 < Num_Receivers; rec_0++) {
            if(rec_list[rec_0] == NULL) continue;
            Uint32 receiver_bias_pos = 0;
            for(; receiver_bias_pos < NUM_RECEIVER; receiver_bias_pos++) {
                if(Receiver_List[receiver_bias_pos] == rec_list[rec_0])
                    break;
            }
            if(receiver_bias_pos == NUM_RECEIVER) continue;

            for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
                Real64 bias = workspace[NUM_PARAMETERS + NUM_RECEIVER*type
                                        +receiver_bias_pos];
                // Now where is the station error located?
                Real64 sta_err = workspace[Max_Number_Workspace
                                           -3*NUM_RECEIVER
                                           *GPS_MAX_POSSIBLE_RECEIVER_TYPE
                                           + NUM_RECEIVER*type
                                           + receiver_bias_pos];
                //printf("At MJD %10.4f got bias %10.2E for rec %4u type %1u with err %10.2E\n", MJD, bias, rec_0, type, sta_err);
                if((bias == 0.0) && (sta_err == 0.0)) continue;
                station_sum[Num_Receivers*type+rec_0]    += bias;
                station_sumsqr[Num_Receivers*type+rec_0] += bias*bias;
                station_error[Num_Receivers*type+rec_0]  += sta_err;
                station_count[Num_Receivers*type+rec_0]  += 1;
            } // for type
        } // for all receivers
        for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++) {
            if(satellite_bias_workspace[s] != 0.0) {
                satellite_sum[s] += satellite_bias_workspace[s];
                satellite_count[s]++;
            }
        }
    } // for MJD over time

    // loop over the receivers
    for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
        if(rec_list[receiver] == NULL) continue;
        // Get average bias level
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Uint32 count = station_count[Num_Receivers*type+receiver];
            Real64 average = 0.0;
            Real64 std_dev = 0.0;
            Real64 mean_error = 0.0;
            if(count > 0) {
                average = station_sum[Num_Receivers*type+receiver] / count;
                if(average == 0.0) continue;
                if(count > 1) {
                    std_dev = (station_sumsqr[Num_Receivers*type+receiver]
                               - station_sum[Num_Receivers*type+receiver]
                               *station_sum[Num_Receivers*type+receiver] / count)
                        / (count-1);
                    // This is the RMS level, find the uncertainty in the average
                    std_dev /= Real64(count);
                    std_dev = (std_dev > 0.0) ? sqrt(std_dev) : 0.0;
                }
                mean_error = station_error[Num_Receivers*type+receiver] / count;
                printf("Got station %4u average %10.2E %10.2E %10.2E from %u times\n", Num_Receivers*type+receiver, average, std_dev, mean_error, count);
                Real64 err = ((Apply_RMS_To_BIAS)) ? mean_error + std_dev : 0.0;
                rec_list[receiver]->
                    station_obs.adjust_bias(average,
                                            err,
                                            type,
                                            true);
            }
        }
    }
    // Loop over satellites
    for(Uint32 s=0; s < GPS_MAX_POSSIBLE_SATELLITES; s++) {
        if((satellite_count[s])) {
            Real64 average_bias = satellite_sum[s] / satellite_count[s];
            printf("Got sat %3u bias %10.2E\n", s, average_bias);
            for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
                if(rec_list[receiver] == NULL) continue;
                rec_list[receiver]->
                    station_obs.adjust_satellite_bias(s, average_bias, 0.0);
            }
        }
    }
    

    selection_criteria = original_user_criteria;
    return 0.0;
}

















//_TITLE  correct_receiver_biases_track_force --force track biases
Real64 GPS_collection::correct_receiver_biases_track_force(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const GPS_Fit_Type_Enum Fit_Type,//I The type of fit to perform
    Uint32 NUM_PARAMETERS,      //I  number of polynomial parameters to allow
    const Real64 Averaging_Time,//I  Averaging time of the data in s
    const bool Apply_RMS_To_BIAS//I Apply the RMS to correct bias level?
    )
// Real64 GPS_collection::correct_receiver_biases_track_force
//                                O  return value
//                                   normally this indicates the abs(max) bias
//	                             but if the system is not fully initialized
//                                   the value is GPS_NOT_INITIALIZED_VALUE

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	The GPS receivers and satellites typically have biases between the
//	different frequencies in the receivers/transmitters (delays) which
//	result in bias delay values, which in turn affect the measured
//	ionospheric delay values.  These bias levels seem to be relatively
//	constant in time, although there may be temperature and other effects
//	at low levels.

//	In general, the satellite bias delays are normally known through
//	analysis from GPS groups around the world.  These bias corrections
//	should have already been applied to the data which comes into the
//	GPS_collection object.

//	The station receiver biases, on the other hand, are often not corrected.
//	(Some stations have bias correction values available, some do not.)
//	The bias levels are often as much as 10 TECU, so it is extremely
//	important to correct these biases.

//	Furthermore, it seems that the individual tracks of satellites across
//	the sky for each receiver have different bias levels resulting from the
//	uncertainty in determining the phase cycle correction term.  This
//	function therefore fits a bias to each satellite track for each station.

//	This routine calculates biases for all receivers at once, using a
//	global method (hopefully something like spherical harmonics).  It
//	only has to go through the receiver list once, but the number of
//	datapoints and parameters is huge, so gradient minimization least
//	squares is hopefully used.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Apr 19  James M Anderson  --JIVE  start

//_END

{
    if((time_arr == NULL) || (sat_pos == NULL) || (rec_list == NULL))
        return GPS_NOT_INITIALIZED_VALUE;

    // Ok, first off, save the user's selection criteria, as I need my own.
    GPS_criteria original_user_criteria = selection_criteria;
    
    // Now, I need some workspace
    std::vector<Real64> track_sum(Satellite_Track_Max_Tracks);
    std::vector<Real64> track_sumsqr(Satellite_Track_Max_Tracks);
    std::vector<Real64> track_error(Satellite_Track_Max_Tracks);
    std::vector<Uint32> track_count(Satellite_Track_Max_Tracks);

    Real64 start_MJD = time_arr->get_time(0)
        + PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;
    Real64 end_MJD = time_arr->get_time(time_arr->NUM_TIMES-1)
        - PHYSICAL_CONSTANTS::DAYS_PER_SECOND * Averaging_Time * 0.5;

    // setup new criteria for this receiver
    Ionosphere_Theoretical_Model_Enum theo_use =
        selection_criteria.Theo_Model_Type;
    if((selection_criteria.Bias_Fit_Type == Use_Main_Without_Theoretical)
       ||(selection_criteria.Bias_Fit_Type == Use_Global_Without_Theoretical)
       ||(selection_criteria.Bias_Fit_Type == Use_Global_Track_Without_Theo)) {
        theo_use = None;
    }
    selection_criteria = GPS_criteria(Fit_Type,
                                      2.0*M_PI,
                                      20.0*M_DEG2RAD, // elevation angle
                                      2E7, // receiver distance
                                      2E7, // iono pierce
                                      300E3,  // iono height
                                      Averaging_Time * 0.5,
                                      NUM_PARAMETERS,
                                      2, // num heights
                                      2, // NUM_TIME_TERMS
                                      theo_use,
                                      selection_criteria.Bias_Fit_Type
                                      );
    // Set up the reference location as 0 lat, 0 lon
    Station_Reference station(0.0,0.0);
    Space_Unit_Vector direction = station.make_unit_vector();

    // Get the list of nearby receivers, to order the list as the
    // fitting routine does.
    GPS_receiver** Receiver_List = NULL;
    Uint32 NUM_RECEIVER = UINT32_MAX;
    get_nearby_receivers(station, Receiver_List, NUM_RECEIVER);
    if(NUM_RECEIVER == 0) {
        // Hey, this should never happen
        selection_criteria = original_user_criteria;
        return GPS_NOT_INITIALIZED_VALUE;
    }
    if(NUM_PARAMETERS > 3*NUM_RECEIVER) NUM_PARAMETERS = 3*NUM_RECEIVER;
    selection_criteria.MAX_NUM_PARAMETERS = NUM_PARAMETERS;


    for(Real64 MJD = start_MJD; MJD < end_MJD;
        MJD += (Averaging_Time * 0.25 + 30.0)
            * PHYSICAL_CONSTANTS::DAYS_PER_SECOND) {
        clear_fake_track_num();
        // do the calculation
        GPS_collection_return ionosphere =
            get_GPS_prediction(station,
                               direction,
                               MJD,
                               selection_criteria.Default_Fit_Type,
                               false, true,
                               false,
                               false,
                               true, // fit object bias
                               NULL);
        if((ionosphere.STEC == GPS_BAD_DATA_VALUE)
           || (ionosphere.STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
        // Find the position of the receivers in the temporary receiver list
        Uint32 NUM_TRACKS = get_fake_track_max();
        fprintf(stderr, "Found %u fake tracks\n", NUM_TRACKS);
        for(Uint32 t=0; t < NUM_TRACKS; t++) {
            Uint32 rec_ID, real_track_num;
            Sint16 sat_ID, track_ID;
            get_real_track_info(t, rec_ID, sat_ID, track_ID, real_track_num);
            fprintf(stderr, "%u %u %d %d %u\n", t, rec_ID, sat_ID, track_ID, real_track_num);
            if(rec_ID == UINT32_MAX) continue;
            Real64 bias = workspace[NUM_PARAMETERS + t];
            // Now where is the station error located?
            Real64 sta_err = workspace[Max_Number_Workspace
                                       - 3*Satellite_Track_Max_Tracks
                                       + t];
            //printf("At MJD %10.4f got bias %10.2E for rec %4u sat %3d track %3d with err %10.2E\n", MJD, bias, rec_ID, sat_ID, track_ID, sta_err);
            if((bias == 0.0) && (sta_err == 0.0)) continue;
            track_sum[real_track_num]    += bias;
            track_sumsqr[real_track_num] += bias*bias;
            track_error[real_track_num]  += sta_err;
            track_count[real_track_num]  += 1;
        } // for all tracks
        fprintf(stderr,"Done with time %.5f\n", MJD);
    } // for MJD over time

    // loop over the tracks
    std::vector<Uint32> track_num(GPS_MAX_POSSIBLE_SATELLITES);
    for(Uint32 r=0; r < Num_Receivers; r++) {
        if((rec_list[r])) {
            Uint32* pos = satellite_track_main_ID +r*GPS_MAX_POSSIBLE_SATELLITES;
            rec_list[r]->station_obs.total_tracks(&(track_num[0]));
            for(Sint16 s=0; Uint32(s) < GPS_MAX_POSSIBLE_SATELLITES; s++) {
                for(Sint16 t=0; Uint32(t) < track_num[s]; t++) {
                    Uint32 this_track = pos[s] + t;
                    Uint32 count = track_count[this_track];
                    if((count == 0) || (track_sum[this_track] == 0.0)) {
                        // no data for this track?
                        // Check to set error very high
                        if((Apply_RMS_To_BIAS))
                            rec_list[r]->
                                station_obs.adjust_satellite_bias2(s, t,0.0,
                                                                   1.0E24);
                        continue;
                    }
                    Real64 average = track_sum[this_track] / count;
                    Real64 std_dev = 0.0;
                    if(count > 1) {
                        std_dev = (track_sumsqr[this_track]
                                   - track_sum[this_track]
                                   *track_sum[this_track] / count)
                            / (count-1);
                        // This is the RMS level, find the uncertainty in the average
                        std_dev /= Real64(count);
                        std_dev = (std_dev > 0.0) ? sqrt(std_dev) : 0.0;
                    }
                    Real64 mean_error = track_error[this_track] / count;
                    printf("Got station %4u %3d %3d average %10.2E %10.2E %10.2E from %3u times\n", r, s, t, average, std_dev, mean_error, count);
                    Real64 err = ((Apply_RMS_To_BIAS)) ? mean_error + std_dev : 0.0;
                    rec_list[r]->
                        station_obs.adjust_satellite_bias2(s, t,average,err);
                    
                }
            }
        }
    }

    selection_criteria = original_user_criteria;
    return 0.0;
}














//_TITLE  force_absolute_night_bias --force stations to have bias correction
Sint32 GPS_collection::force_absolute_night_bias(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    GPS_receiver* receiver,    // B  The receiver to bias correct
    const Real64 Night_Bias_Level,
                               // I  The night-time vertical total electron
                               //    content to force to receiver to, in units
                               //    of electrons m^{-2}
    const Sint32 bias_status   // I  status of bias correction to apply
    )
// Sint32 GPS_collection::force_absolute_night_bias
//                                O  return value
//                                   normally this is 0
//	                             but if something bad happens, -1
//                                   -2 for NULL receiver
//                                   -3 not enough satellites

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	Finding the bis level correction for GPS receivers is difficult.
//	Normally there are several stations which have been corrected
//	for DCB bias stuff from external information.  But there are now
//	many, many receivers which have no bias data available.

//	If a dataset collection has no primary bias receivers, then something
//	must be done.  This function will find the minimum ionosphere value
//	for the entire time length of observation.  This normally happens during
//	the night.  It will then make a bias correction to force the receiver
//	minimum to be the user-supplied night value.  This night ionosphere
//	value (Vertical TEC) should normally be about 2 to 10 TECU for
//	Solar min to Solar Max and by latitude region.  


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 14  James M Anderson  --JIVE  start

//_END

{
    if(receiver==NULL) return -2;

    {
        selection_criteria.NUM_TIME_TERMS = 2;
        Real64 retval = correct_receiver_biases_low(multiple_2D_time, 20,
                                                    3600.5,
                                                    false, false, false, false);
        if(retval != 0.0) return -1;
        receiver->station_obs.force_bias_status(0,bias_status);
        return 0;
    }


    
    Real64 min_VTEC_total = 1E300;
    for(Uint32 t=0; t < time_arr->NUM_TIMES; t++) {
        Real64 sum = 0.0;
        Real64 sum_wt = 0.0;
        Real64 min_VTEC = 1E300;
        Real64 max_VTEC = -1E300;
        Real64 min_wt = 0.0;
        Real64 max_wt = 0.0;
        Uint32 count = 0;
        for(Uint32 s=0; s < receiver->station_obs.NUM_SATELLITES;s++) {
            // get this satellite
            Sint16 sat_num;
            Real64 STEC;
            Real64 sigma;
            receiver->station_obs.get_data(t, s, &sat_num, &STEC, &sigma);
            if(sat_num < 0) continue;
            if(sat_num >= 100) continue; // only do GPS satellites
            if((STEC == GPS_BAD_DATA_VALUE)
               || (STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
            if(sigma <= 0.0) continue;
            Real64 weight = 1.0 / sigma;
            Space_Unit_Vector satellite =
                (sat_pos->get_sat(t, Uint32(sat_num))
                 - receiver->station_position).make_unit_vector();
            Space_Unit_Vector station =
                receiver->station_position.make_unit_vector();
            Real64 zenith_dist = station.Angular_Separation(satellite);
            Real64 El = M_PI_2 - zenith_dist;
            Real64 VTEC =
                get_simple_VTEC_scaling(El,
                                        receiver->station_position.Elevation(),
                                        selection_criteria.Default_Iono_Height)
                * STEC;
            sum += VTEC * weight;
            sum_wt += weight;
            count++;
            if(VTEC < min_VTEC) {
                min_VTEC = VTEC;
                min_wt = weight;
            }
            if(VTEC > max_VTEC) {
                max_VTEC = VTEC;
                max_wt = weight;
            }
        }
        if(count > 2) {
            // enough observations to make this useful
            sum -= min_VTEC * min_wt + max_VTEC * max_wt;
            sum_wt -= min_wt + max_wt;
            Real64 average = sum / sum_wt;
            if(average < min_VTEC_total) min_VTEC_total = average;
            fprintf(stderr, "Forcing Bias time %4u %11.3E %11.3E\n",t, average,min_VTEC_total); 
        }
    } // for t over all times
    if(min_VTEC_total == 1E300) return -3;
    // Ok, I now have the minimum.  Do the bias correction
    Real64 bias = min_VTEC_total - Night_Bias_Level;
    fprintf(stderr, "Final bias correction %11.3E\n", bias);
    receiver->station_obs.adjust_bias(bias,0.0,0,false);
    // Force the calibration status to the user specified value
    receiver->station_obs.force_bias_status(0,bias_status);
    return 0;
}







//_TITLE  enforce_all_bias_corrected --make sure all receivers corrected
Sint32 GPS_collection::enforce_all_bias_corrected(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    void
    )
// Sint32 GPS_collection::enforce_all_bias_corrected
//                                O  return value
//                                   normally this is 0
//	                             but if something bad happens, -1
//                                   -2 for general calibration failure

//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program
//	Run through the list of receivers, and if someone does not have a bias
//	correction applied, apply one.

//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 14  James M Anderson  --JIVE  start

//_END

{
//     for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
//         if(rec_list[receiver] == NULL) continue;
//         bool has_correction = false;
//         for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
//             Sint32 bias_valid =
//                 rec_list[receiver]->station_obs.get_bias_valid(type);
//             if(bias_valid > 0) {
//                 has_correction = true;
//                 break;
//             }
//         }
//         if(!has_correction) {
//             fprintf(stderr, "Warning: Receiver %u left with uncalibrated bias --- fixing\n");
//             force_absolute_night_bias(rec_list[receiver],
//                                       2.0*TECU_SI,
//                                       3);
//             // Now run through and try to propagate
            
//             for(Uint32 i=0; i < 9; i++) {
//                 printf("Starting uncalibrated bias correction fix\n");
//                 Real64 retval = correct_receiver_biases_low(multiple_2D, 10,
//                                                             450.0,
//                                                             false, false,
//                                                             true, false);
//                 if(retval != 0.0) return -2;
//             }
//         }
//     } // for receiver over receivers
    bool all_corrected = true;
    for(Uint32 receiver = 0; receiver < Num_Receivers; receiver++) {
        if(rec_list[receiver] == NULL) continue;
        for(Uint32 type = 0; type < GPS_MAX_POSSIBLE_RECEIVER_TYPE; type++) {
            Sint32 bias_valid =
                rec_list[receiver]->station_obs.get_bias_valid(type);
            if(bias_valid == 0) {
                all_corrected = false;
                break;
            }
        }
        if(!all_corrected) break;
    }
    if(!all_corrected) {
        fprintf(stderr, "Starting uncalibrated bias correction fix\n");
        Real64 retval = correct_receiver_biases_force(
            multiple_2D_time,
            20,
            7200.5,
            false,
            false);
        if(retval != 0.0) return -2;
    }

    return 0;
}

















//_TITLE  dump_receiver_to_disk --dump a receiver's data to disk
Sint32 GPS_collection::dump_receiver_to_disk(
//_ARGS  TYPE           VARIABLE I/O DESCRIPTION
    const GPS_receiver* const receiver,
                               // I  The receiver to dump
    const char filename[]      // I  the name of the file to dump to
    )
// Sint32 GPS_collection::dump_receiver_to_disk
//                                O  return value
//                                   normally this is 0
//	                             but if something bad happens, -1
//                                   -2 for NULL receiver


//_USER  any user input?

//_VARS  TYPE           VARIABLE I/O DESCRIPTION
//       put globals used here

//_DESC  full description of program


//_FILE  files used and logical units used

//_LIMS  design limitations

//_BUGS  known bugs

//_CALL  list of calls

//_KEYS  

//_HIST  DATE NAME PLACE INFO
//	2007 Mar 14  James M Anderson  --JIVE  start

//_END

{
    if(receiver==NULL) return -2;
    FILE* fp = fopen(filename, "w");
    for(Uint32 t=0; t < time_arr->NUM_TIMES; t++) {
        for(Uint32 s=0; s < receiver->station_obs.NUM_SATELLITES;s++) {
            // get this satellite
            Sint16 sat_num;
            Real64 STEC;
            Real64 sigma;
            receiver->station_obs.get_data(t, s, &sat_num, &STEC, &sigma);
            if(sat_num < 0) continue;
            //if(sat_num >= 100) continue; // only do GPS satellites
            if((STEC == GPS_BAD_DATA_VALUE)
               || (STEC == GPS_NOT_INITIALIZED_VALUE)) continue;
            if(sigma <= 0.0) continue;
            Space_Unit_Vector satellite =
                (sat_pos->get_sat(t, Uint32(sat_num))
                 - receiver->station_position).make_unit_vector();
            Space_Unit_Vector station =
                receiver->station_position.make_unit_vector();
            Real64 zenith_dist = station.Angular_Separation(satellite);
            Real64 El = M_PI_2 - zenith_dist;
            LatLon_Cart AltAz =
                        receiver->station_position.convert_RADec_to_AltAz(satellite);
            //Real64 El   = AltAz.Lat();
            Real64 Az   = AltAz.Lon();
            Real64 VTEC =
                get_simple_VTEC_scaling(El,
                                        receiver->station_position.Elevation(),
                                        selection_criteria.Default_Iono_Height)
                * STEC;
            Real64 MJD_obs = time_arr->get_time(t);
            fprintf(fp,"%.5f %4.4s %4u %10.3E %10.3E %8.1E %7.2f %6.2f\n",
                    MJD_obs,
                    receiver->station_name(),
                    sat_num,
                    STEC,
                    VTEC,
                    sigma,
                    Az*M_RAD2DEG,
                    El*M_RAD2DEG
                    );
        } // for s over all satellites
    } // for t over all times
    fclose(fp);
    return 0;
}








    

}  // end namespace


