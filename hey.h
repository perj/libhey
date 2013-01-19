
struct hey;

enum hey_af_flag
{
	hey_aff_inet = 0x1,
	hey_aff_inet6 = 0x2,
	hey_aff_inet6_mapped = 0x4,
	hey_aff_unix = 0x8,
};

enum hey_af
{
	hey_af_any                  = 0xff,
	hey_af_any_inet             = hey_aff_inet | hey_aff_inet6 | hey_aff_inet6_mapped,
	hey_af_inet                 = hey_aff_inet,
	hey_af_inet6                = hey_aff_inet6,
	hey_af_unix                 = hey_aff_unix,
	hey_af_inet6_mapped         = hey_aff_inet6 | hey_aff_inet6_mapped,
	hey_af_inet6_mapped_or_unix = hey_aff_inet6 | hey_aff_inet6_mapped | hey_aff_unix,
};

struct hey *hey_create(enum hey_af af, int use_srv);
void hey_free(struct hey *hey);

void hey_set_cache_ttl(struct hey *hey, int seconds);
void hey_clear_cache(struct hey *hey, const char *host);

int hey_connect(struct hey *hey, const char *host, const char *serv, int timeout);
