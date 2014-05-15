/* Common output layer interface */

/* Each output layer must provide methods for 
 * storing:
 * - Nodes (Points of interest etc)
 * - Way geometries
 * Associated tags: name, type etc. 
*/

#ifndef OUTPUT_H
#define OUTPUT_H

#include "middle.hpp"
#include "keyvals.hpp"
#include "reprojection.hpp"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

/* Variants for generation of hstore column */
/* No hstore column */
#define HSTORE_NONE 0
/* create a hstore column for all tags which do not have an exclusive column */
#define HSTORE_NORM 1
/* create a hstore column for all tags */
#define HSTORE_ALL 2

/* Scale is chosen such that 40,000 * SCALE < 2^32          */
static const int DEFAULT_SCALE = 100;

struct output_options {
  output_options():scale(DEFAULT_SCALE){};

  const char *conninfo;  /* Connection info string */
  const char *prefix;    /* prefix for table names */
  int scale;       /* scale for converting coordinates to fixed point */
  int projection;  /* SRS of projection */
  int append;      /* Append to existing data */
  int slim;        /* In slim mode */
  int cache;       /* Memory usable for cache in MB */
  struct middle_t *mid;  /* Mid storage to use */
  struct output_t *out;  /* Output type used */
  const char *tblsmain_index;     /* Pg Tablespace to store indexes on main tables */
  const char *tblsslim_index;     /* Pg Tablespace to store indexes on slim tables */
  const char *tblsmain_data;     /* Pg Tablespace to store main tables */
  const char *tblsslim_data;     /* Pg Tablespace to store slim tables */
  const char *style;     /* style file to use */
  int expire_tiles_zoom;	/* Zoom level for tile expiry list */
  int expire_tiles_zoom_min;	/* Minimum zoom level for tile expiry list */
  const char *expire_tiles_filename;	/* File name to output expired tiles list to */
  int enable_hstore; /* add an additional hstore column with objects key/value pairs */
  int enable_hstore_index; /* add an index on the hstore column */
  int enable_multi; /* Output multi-geometries intead of several simple geometries */
  const char** hstore_columns; /* list of columns that should be written into their own hstore column */
  int n_hstore_columns; /* number of hstore columns */
  int keep_coastlines;
  int parallel_indexing;
  int alloc_chunkwise;
  int num_procs;
  int droptemp; /* drop slim mode temp tables after act */
  int unlogged; /* use unlogged tables where possible */
  int hstore_match_only; /* only copy rows that match an explicitly listed key */
  int flat_node_cache_enabled;
  int excludepoly;
  const char *flat_node_file;
  const char *tag_transform_script;
};

struct output_t : public boost::noncopyable {
    virtual ~output_t();

    virtual int start(const struct output_options *options, boost::shared_ptr<reprojection> r) = 0;
    virtual int connect(const struct output_options *options, int startTransaction) = 0;
    virtual void stop() = 0;
    virtual void cleanup(void) = 0;
    virtual void close(int stopTransaction) = 0;

    virtual int node_add(osmid_t id, double lat, double lon, struct keyval *tags) = 0;
    virtual int way_add(osmid_t id, osmid_t *nodes, int node_count, struct keyval *tags) = 0;
    virtual int relation_add(osmid_t id, struct member *members, int member_count, struct keyval *tags) = 0;

    virtual int node_modify(osmid_t id, double lat, double lon, struct keyval *tags) = 0;
    virtual int way_modify(osmid_t id, osmid_t *nodes, int node_count, struct keyval *tags) = 0;
    virtual int relation_modify(osmid_t id, struct member *members, int member_count, struct keyval *tags) = 0;

    virtual int node_delete(osmid_t id) = 0;
    virtual int way_delete(osmid_t id) = 0;
    virtual int relation_delete(osmid_t id) = 0;
};

unsigned int pgsql_filter_tags(enum OsmType type, struct keyval *tags, int *polygon);

#endif
