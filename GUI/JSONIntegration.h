/** \file JSONIntegration.cpp
* Contains the declaration of functions that transforms the JSON file into the
* class objects and vice-versa.
*/

#ifndef JSONINTEGRATION_H
#define JSONINTEGRATION_H

#include "json.hpp"
#include "TStructure.h"
#include "TNode.h"
#include "TMaterial.h"
#include "TSupport.h"
#include "TElement.h"
#include "TDistributedLoad.h"
#include "TElementEndMoment.h"
#include "TNodalLoad.h"

//! Prints a JSON object to std::cout.
/*!
\param J the address of the JSON object to be printed.
*/
void printJSON(const nlohmann::json& J);

//! Converts JSON into structure and loads objects.
/*!
\param J the address of the input JSON object.
\param Parent the pointer to the TStructure object to be filled.
\param NodalLoads the address of the TNodalLoad vector to be filled.
\param DistributedLoads the address of the TDistributedLoad vector to be filled.
\param EndMoments the address of the TElementEndMoment vector to be filled.
*/
void importData(const nlohmann::json &J, TStructure *Parent,
	std::vector<TNodalLoad> &NodalLoads,
	std::vector<TDistributedLoad> &DistributedLoads,
	std::vector<TElementEndMoment> &EndMoments);

//! Converts a TMaterial object to JSON.
/*!
\param J the adress of the JSON object.
\param M the address ot the TMaterial object.
*/
void to_json(nlohmann::json& J, const TMaterial& M);

//! Converts a JSON object to a TMaterial one.
/*!
\param J the adress of the JSON object.
\param M the address ot the TMaterial object.
*/
void from_json(const nlohmann::json& J, TMaterial& M);

//! Converts a TNode object to JSON.
/*!
\param J the adress of the JSON object.
\param N the address ot the TNode object.
*/
void to_json(nlohmann::json& J, const TNode& N);

//! Converts a JSON object to a TNode one.
/*!
\param J the adress of the JSON object.
\param N the address ot the TNode object.
*/
void from_json(const nlohmann::json& J, TNode& N);

//! Converts a TSupport object to JSON.
/*!
\param J the adress of the JSON object.
\param S the address ot the TSupport object.
*/
void to_json(nlohmann::json& J, const TSupport& S);

//! Converts a JSON object to a TSupport one.
/*!
\param J the adress of the JSON object.
\param S the address ot the TSupport object.
*/
void from_json(const nlohmann::json& J, TSupport& S);

//! Converts a TElement object to JSON.
/*!
\param J the adress of the JSON object.
\param E the address ot the TElement object.
*/
void to_json(nlohmann::json& J, const TElement& E);

//! Converts a JSON object to a TElement one.
/*!
\param J the adress of the JSON object.
\param E the address ot the TElement object.
*/
void from_json(const nlohmann::json& J, TElement& E);

//! Converts a TNodalLoad object to JSON.
/*!
\param J the adress of the JSON object.
\param NL the address ot the TNodalLoad object.
*/
void to_json(nlohmann::json& J, const TNodalLoad& NL);

//! Converts a JSON object to a TNodalLoad one.
/*!
\param J the adress of the JSON object.
\param NL the address ot the TNodalLoad object.
*/
void from_json(const nlohmann::json& J, TNodalLoad& NL);

//! Converts a TDistributedLoad object to JSON.
/*!
\param J the adress of the JSON object.
\param DL the address ot the TDistributedLoad object.
*/
void to_json(nlohmann::json& J, const TDistributedLoad& DL);

//! Converts a JSON object to a TDistributedLoad one.
/*!
\param J the adress of the JSON object.
\param DL the address ot the TDistributedLoad object.
*/
void from_json(const nlohmann::json& J, TDistributedLoad& DL);

//! Converts a TElementEndMoment object to JSON.
/*!
\param J the adress of the JSON object.
\param EEM the address ot the TElementEndMoment object.
*/
void to_json(nlohmann::json& J, const TElementEndMoment& EEM);

//! Converts a JSON object to a TElementEndMoment one.
/*!
\param J the adress of the JSON object.
\param EEM the address ot the TElementEndMoment object.
*/
void from_json(const nlohmann::json& J, TElementEndMoment& EEM);

#endif // JSONINTEGRATION_H
