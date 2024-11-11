#include <stdio.h>
#include <spiffs_partition.h>
#include <string.h>



/* ********************************************
            FUNCTIONS IMPLEMENTATIONS
   ******************************************** */

void Spiffs( spiffs_t * self, esp_vfs_spiffs_conf_t * cfg_p, char * filenames_p[], int n_files_p )
{
    self->cfg.base_path              = cfg_p->base_path;
    self->cfg.partition_label        = cfg_p->partition_label;
    self->cfg.max_files              = cfg_p->max_files;
    self->cfg.format_if_mount_failed = cfg_p->format_if_mount_failed;
    self->n_files                    = n_files_p;
    self->filename                   = ( char ** ) malloc( n_files_p * sizeof( char * ) );
    for (int i = 0; i < n_files_p; i++)
        self->filename[ i ] = strdup( filenames_p[ i ] );
    self->total                      = 0;
    self->used                       = 0;
    self->set_cfg                    = set_cfg;
    self->info                       = info;
    self->get_partition              = get_partition;
    self->erase_partition            = erase_partition;
    spiffs_init( self );
}

void spiffs_init( spiffs_t * self )
{
    self->set_cfg( self );
    if( self->get_partition( self ) != NULL )
        self->erase_partition( self, self->get_partition( self ), 0x0, self->get_partition( self )->size );
    printf( "Spiffs partition initialized.\n" );
}
