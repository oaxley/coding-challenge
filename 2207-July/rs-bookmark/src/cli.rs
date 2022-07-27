/* -*- coding: utf-8 -*-
 * vim: set ft=rust
 *
 * This source file is subject to the Apache License 2.0
 * that is bundled with this package in the file LICENSE.txt.
 * It is also available through the Internet at this address:
 * https://opensource.org/licenses/Apache-2.0
 *
 * @author	Sebastien LEGRAND
 * @license	Apache License 2.0
 *
 * @brief	CLI function
 */

//----- imports
use clap::{arg, Command, command};


//----- functions
pub fn command_line<'a>() -> Command<'a> {
    command!()
        .propagate_version(true)
        .about("Bookmark manager")
        .subcommand_required(true)
        .arg_required_else_help(true)
        .allow_external_subcommands(true)

        .subcommand(
            Command::new("store")
                .about("Manage bookmark store")
                .subcommand(
                    Command::new("new")
                        .about("Create a new store")
                        .arg(arg!(<store> "store name"))
                        .arg_required_else_help(true)
                )

                .subcommand(
                    Command::new("export")
                        .about("Export the content of the store")
                        .arg(arg!(<store> "store name"))
                        .arg_required_else_help(true)
                        .arg(arg!(-r --regex <REGEX> "Regex to select particular URLs, Descriptions or Tags").required(false))
                        .arg(arg!(-f --format <FORMAT> "Optional output format").required(false))
                )

                .subcommand(
                    Command::new("test")
                        .about("Test the validity of bookmarks in the store")
                        .arg(arg!(<store> "store name"))
                        .arg_required_else_help(true)
                        .arg(arg!(-r --regex <REGEX> "Regex to select particular URLs, Descriptions or Tags").required(false))
                        .arg(arg!(-f --format <FORMAT> "Optional output format").required(false))
                )
        )

        .subcommand(
            Command::new("url")
                .about("Manage bookmarks")
                .subcommand(
                    Command::new("new")
                        .about("Create a new bookmark")
                        .arg(arg!(<url> "URL for this bookmark"))
                        .arg_required_else_help(true)
                        .arg(arg!(-d --descr <DESCRIPTION> "Add a description to this URL").required(false))
                        .arg(arg!(-t --tags <TAGS> "Add tags to this URL")
                            .required(false)
                            .multiple(true)
                        )
                        .arg(arg!(-s --store <STORE> "Bookmark store where to add the URL").required(false))
                )

                .subcommand(
                    Command::new("update")
                        .about("Update an URL")
                        .arg(arg!(<ID> "ID for this URL"))
                        .arg_required_else_help(true)
                        .arg(arg!(-u --url <URL> "Change the URL link").required(false))
                        .arg(arg!(-d --descr <DESCRIPTION> "Change the description").required(false))
                        .arg(arg!(-t --tags <TAGS> "Change the tags")
                            .required(false)
                            .multiple(true)
                        )
                        .arg(arg!(-s --store <STORE> "Bookmark store").required(false))
                )

                .subcommand(
                    Command::new("print")
                        .about("Print details on an URL")
                        .arg(arg!(<ID> "ID for this URL"))
                        .arg_required_else_help(true)
                        .arg(arg!(-f --format <FORMAT> "Optional output format").required(false))
                        .arg(arg!(-s --store <STORE> "Bookmark store").required(false))
                )

                .subcommand(
                    Command::new("delete")
                        .about("Delete an URL")
                        .arg(arg!(<ID> "ID for this URL"))
                        .arg_required_else_help(true)
                        .arg(arg!(-s --store <STORE> "Bookmark store").required(false))
                )

                .subcommand(
                    Command::new("search")
                        .about("Search for an URL")
                        .arg(arg!(-u --url <URL> "Change the URL link").required(false))
                        .arg(arg!(-d --descr <DESCRIPTION> "Change the description").required(false))
                        .arg(arg!(-t --tags <TAGS> "Change the tags")
                            .required(false)
                            .multiple(true)
                        )
                        .arg(arg!(-f --format <FORMAT> "Optional output format").required(false))
                        .arg(arg!(-s --store <STORE> "Bookmark store").required(false))
                )
        )
}