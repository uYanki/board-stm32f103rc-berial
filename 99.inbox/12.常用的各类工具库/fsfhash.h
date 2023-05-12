/* Hash table declarations.
   Copyright (C) 2000 Free Software Foundation, Inc.
 */

#ifndef HASH_H
#define HASH_H
#ifdef __cplusplus
extern "C" {
#endif
struct hash_table;

struct hash_table *hash_table_new (int,
					   uint32_t (*) (const void *),
					   int (*) (const void *,
						    const void *));
void hash_table_destroy (struct hash_table *);

void *hash_table_get  (const struct hash_table *, const void *);
int hash_table_get_pair (const struct hash_table *, const void *,
				 void *, void *);
int hash_table_contains (const struct hash_table *, const void *);

void hash_table_put (struct hash_table *, const void *, void *);
int hash_table_remove (struct hash_table *, const void *);
void hash_table_clear (struct hash_table *);

void hash_table_map (struct hash_table *,
			     int (*) (void *, void *, void *),
			     void *);
int hash_table_count (const struct hash_table *);

struct hash_table *make_string_hash_table (int);
struct hash_table *make_nocase_string_hash_table (int);

uint32_t hash_pointer (const void *);

/* From XEmacs, and hence from Dragon book. */

#define GOOD_HASH 65599 /* prime number just over 2^16; Dragon book, p. 435 */
#define HASH2(a,b)               (GOOD_HASH * (a)                     + (b))
#define HASH3(a,b,c)             (GOOD_HASH * HASH2 (a,b)             + (c))
#define HASH4(a,b,c,d)           (GOOD_HASH * HASH3 (a,b,c)           + (d))
#define HASH5(a,b,c,d,e)         (GOOD_HASH * HASH4 (a,b,c,d)         + (e))
#define HASH6(a,b,c,d,e,f)       (GOOD_HASH * HASH5 (a,b,c,d,e)       + (f))
#define HASH7(a,b,c,d,e,f,g)     (GOOD_HASH * HASH6 (a,b,c,d,e,f)     + (g))
#define HASH8(a,b,c,d,e,f,g,h)   (GOOD_HASH * HASH7 (a,b,c,d,e,f,g)   + (h))
#define HASH9(a,b,c,d,e,f,g,h,i) (GOOD_HASH * HASH8 (a,b,c,d,e,f,g,h) + (i))

#ifdef __cplusplus
}
#endif
#endif /* HASH_H */
