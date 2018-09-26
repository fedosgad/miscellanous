#define FTYPE double //change this to double for greater precesion

struct interval {
	FTYPE l;
	FTYPE r;
};

typedef struct interval interval;


//Global variables
FTYPE gamma0, rho0, P0, U0;	//vars for initial data
FTYPE gamma3, rho3, P3, U3;
FTYPE C3; 
interval* root_int;
FTYPE* roots;

//Internal variables
static int var;
static int roots_found, total_pos_roots, total_neg_roots;
static interval roots_int_pos, roots_int_neg;
static FTYPE a[7];	//coefs.

static FTYPE X, alpha0, alpha3, e0, e3, C0;	//intermediate vars
static FTYPE n, mu, nu, Z, Y, A, B;

//Getter functions
FTYPE get_a(int i);
FTYPE get_n();
FTYPE get_A();
FTYPE get_B();
interval get_roots_int(int type);
int get_roots_amount(int type);

//Intermediate action functions
void calc_interm_vars();
void calc_coefs();
void estimate_amount();
void initial_guess();
void find_root_ints(int segments);
void find_roots(int method, FTYPE epsilon);

//Function/derivative value calculation
FTYPE fval_1(FTYPE x);	//calculate function value for var. 1
FTYPE fval_2(FTYPE x);	//same for var. 2

//Finding roots
static FTYPE find_root_dichotomy(interval init_guess, FTYPE epsilon);

//Support functions
static FTYPE max_abs(FTYPE* arr, int length);
static FTYPE sq(FTYPE x);
void set_var(int num);
