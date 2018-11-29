#include "definitions.h"
#include "load_obj.h"
#include "transform.h"
#include "utilities.h"
#include <GL/glut.h>
#include <stdio.h>

extern object * _first_object;
extern object * _selected_object;


void updateTransformObject();

/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n");
    printf("\n\n");
}

void print_info(){
	printf("\n############################Info#########################\n\n");
	printf("-----------------------Transforamciones------------------\n");

	printf("Modo de transformacion: ");
	if (mode){
		printf("Camara\n");
	}
	else{
		printf("Objeto\n");
	}

	printf("Matriz de transformacion:\n");
	transform_component * tc = (transform_component *) get_component(_transform_object, COMPONENT_TRANSFORM);
	printMat(tc->undoStack->mat,4,4);


	printf("Modo de transformacion: ");

	switch (transformation){

	case KG_TRANSFORMATIONS_TRANSLATE:
		printf("Translate");
		break;

	case KG_TRANSFORMATIONS_ROTATE:
		printf("Rotate");
		break;

	case KG_TRANSFORMATIONS_SCALE:
		printf("Scale");
		break;

	case KG_TRANSFORMATIONS_REFLECT:
		printf("Reflect");
		break;

	case KG_TRANSFORMATIONS_SHEAR:
		printf("Shear");
		break;
	}

	printf("\n");

	printf("Tipo de transformacion: ");

	if (scope){
		printf("Global\n");
	}else{
		printf("Local\n");
	}

	printf("---------------------------Camara-----------------------\n");
	printf("Modo de visualizacion:\n");

	if (visual_mode){
		printf("Objeto\n");
	}
	else{
		printf("Camara\n");
	}

	printf("Matriz de transformacion de la camara:\n");
	tc = (transform_component *) get_component(_actual_camera, COMPONENT_TRANSFORM);
	printMat(tc->undoStack->mat,4,4);

	printf("Modo de proyeccion: ");

	if (projection_mode){
		printf("Ortho\n");
	}
	else{
		printf("Perspective\n");
	}

}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object *auxiliar_object = 0;
    object *new_camera = 0;
    GLdouble wd,he,midx,midy;



    switch (key) {


    case 'H':
    case 'h':
    	print_info();
    	break;

    case 'N':
    case 'n':
    	new_camera = create_object();

    	transform_component * trans = (transform_component * ) get_component(new_camera,COMPONENT_TRANSFORM);
    	trans->undoStack->mat[12] = 0;
    	trans->undoStack->mat[13] = 0;
    	trans->undoStack->mat[14] = _ortho_x_max;

    	new_camera->next = _first_camera;
    	_first_camera = new_camera;
    	_selected_camera = _first_camera;
    	_actual_camera = _selected_camera;
    	printf("New camera created! \n");
    	break;

    case 'P':
    case 'p':
        if (mode)
            projection_mode = 1 - projection_mode;
        break;


    case 'C':
        _actual_camera = _selected_object;
        visual_mode = 1;
        break;

    case 'Q':
        _actual_camera = _selected_camera;
        visual_mode = 0;
        break;

    case 'O':
    case 'o':
        mode = 0;
        updateTransformObject();
        
        break;

    case 'K':
    case 'k':
        mode = 1;
        updateTransformObject();
        
        break;

    /* Load object */
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        object3d * auxiliar_object3d = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object3d);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /*Insert the new object in the list*/
            auxiliar_object = create_object();
            add_component(auxiliar_object,create_component(COMPONENT_MESH,auxiliar_object3d));
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            //printf("%d\n", auxiliar_object->f_component->component_id);
            //printf("%d\n", auxiliar_object->f_component->next->component_id);

            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        updateTransformObject();
        break;

    /* Change selected object */
    case 9: /* <TAB> */
        _selected_object = _selected_object->next;
        /*The selection is circular, thus if we move out of the list we go back to the first element*/
        if (_selected_object == 0) _selected_object = _first_object;
        updateTransformObject();
        
        break;

    /* Change selected camera */
    case 'c': 
        _selected_camera = _selected_camera->next;
        /*The selection is circular, thus if we move out of the list we go back to the first element*/
        if (_selected_camera == 0) _selected_camera = _first_camera;
        _actual_camera = _selected_camera;
        updateTransformObject();
        
        break;

    /* Delete an object */
    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if (_selected_object == _first_object)
        {
            /*To remove the first object we just set the first as the current's next*/
            _first_object = _first_object->next;
            /*Once updated the pointer to the first object it is save to free the memory*/
            free(_selected_object);
            /*Finally, set the selected to the new first one*/
            _selected_object = _first_object;
        } else {
            /*In this case we need to get the previous element to the one we want to erase*/
            auxiliar_object = _first_object;
            while (auxiliar_object->next != _selected_object)
                auxiliar_object = auxiliar_object->next;
            /*Now we bypass the element to erase*/
            auxiliar_object->next = _selected_object->next;
            /*free the memory*/
            free(_selected_object);
            /*and update the selection*/
            _selected_object = auxiliar_object;

        }
        updateTransformObject();
        break;

    /* Zoom out */
    case '-':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
           zoom(KG_ZOOM_OUT);
        }
        else{
            uniformScale(0);
        }
        break;

    /* Zoom in */
    case '+':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            zoom(KG_ZOOM_IN);
        }
        else{
            uniformScale(1);
        }
        break;


    /* Undo */
    case 26:
 
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            undo();
        }
        break;

    /* Redo */
    case 25:


        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            redo();
        }
        break;


    /* Help */
    case '?':
        print_help();
        break;

    case 27: /* <ESC> */
        exit(0);
        break;

    /* Activate TRANSLATION */
    case 'm':
    case 'M':
        transformation = KG_TRANSFORMATIONS_TRANSLATE;
        break;

    /* Activate ROTATION */
    case 'b':
    case 'B':
        transformation = KG_TRANSFORMATIONS_ROTATE;
        break;

    /* Activate SCALING */
    case 't':
    case 'T':
        transformation = KG_TRANSFORMATIONS_SCALE;
        break;

    /* Activate REFLEXION */
    case 'r':
    case 'R':
        transformation = KG_TRANSFORMATIONS_REFLECT;
        break;

    /* Activate SHEAR MAPPING */
    case 's':
    case 'S':
        transformation = KG_TRANSFORMATIONS_SHEAR;
        break;

    /* Change scope to GLOBAL */
    case 'g':
    case 'G':
        scope = KG_SCOPE_GLOBAL;
        break;

    /* Change scope to LOCAL */
    case 'l':
    case 'L':
        scope = KG_SCOPE_LOCAL;
        break;

    /* Alternatives version for control keys */
    // case 'w':
    // case 'W':
    //     transform(KG_Y_AXIS_POSITIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;

    // case 's':
    // case 'S':
    //     transform(KG_Y_AXIS_NEGATIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;

    // case 'a':
    // case 'A':
    //     transform(KG_X_AXIS_NEGATIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;

    // case 'd':
    // case 'D':
    //     transform(KG_X_AXIS_POSITIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;

    // case 'q':
    // case 'Q':
    //     transform(KG_Z_AXIS_NEGATIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;

    // case 'e':
    // case 'E':
    //     transform(KG_Z_AXIS_POSITIVE);
    //     //printf("%d %d\n",scope,transformation);
    //     break;


    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

/* Function and direction keys control */
void specialKeyboard(int key, int x, int y) {

    switch (key) {

        /* X - AXIS */
        case GLUT_KEY_RIGHT:
            //printf("X_RIGHT \n");
            transform(KG_X_AXIS_POSITIVE);
            break;

        case GLUT_KEY_LEFT:
            //printf("X_LEFT \n");
            transform(KG_X_AXIS_NEGATIVE);
            break;

        /* Y - AXIS */
        case GLUT_KEY_UP:
            //printf("Y_UP \n");
            transform(KG_Y_AXIS_POSITIVE);
            break;

        case GLUT_KEY_DOWN:
            //printf("Y_DOWN \n");
            transform(KG_Y_AXIS_NEGATIVE);
            break;

        /* Z - AXIS */
        case GLUT_KEY_PAGE_UP:
            //printf("Z_UP \n");
            transform(KG_Z_AXIS_POSITIVE);
            break;

        case GLUT_KEY_PAGE_DOWN:
            //printf("Z_DOWN \n");
            transform(KG_Z_AXIS_NEGATIVE);
            break;

        default:
            /*In the default case we just print the code of the key. This is usefull to define new cases*/
            printf("%d %c\n", key, key);
    }

    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();

}

void updateTransformObject(){
    
     _transform_object = (mode) ? _actual_camera : _selected_object;
    
}


